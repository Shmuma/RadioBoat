#ifndef __CONTROLLERCONNECTOR_H__
#define __CONTROLLERCONNECTOR_H__

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTcpSocket>

#include "libusb/libusb.h"


// abstract connector with method
class ControllerConnector : public QObject
{
    Q_OBJECT
public:
    virtual bool connect () = 0;
    virtual void disconnect () = 0;

    virtual void send_raw (char* p, size_t len) = 0;
    virtual size_t recv_raw (char* p, size_t len) = 0;
    virtual bool is_connected () const = 0;

    virtual QString state () const = 0;
signals:
    void connected ();
    void disconnected (const QString& reason);
    void stateChanged (const QString& state);
};


class DirectConnector : public ControllerConnector
{
    Q_OBJECT
private:
    bool _connected;
    struct libusb_context* _ctx;
    struct libusb_device_handle* _handle;

public:
    DirectConnector ()
        : _connected (false)
    {
        libusb_init (&_ctx);
    }

    ~DirectConnector ()
    {
        libusb_exit (_ctx);
    }

    bool connect ();
    void disconnect ();

    void send_raw (char* p, size_t len);
    size_t recv_raw (char* p, size_t len);

    bool is_connected () const
    { return _connected; }

    QString state () const;
};


class NetworkConnector : public ControllerConnector
{
    Q_OBJECT
private:
    QString _host;
    int _port;
    QTcpSocket _sock;

public:
    NetworkConnector (const QString &host, int port)
        : _host (host),
          _port (port)
    {
        QObject::connect (&_sock, SIGNAL (stateChanged (QAbstractSocket::SocketState)),
                          this, SLOT (socketStateChanged (QAbstractSocket::SocketState)));
    }

    bool connect ()
    {
        _sock.connectToHost (_host, _port);
        return true;
    }

    void disconnect ()
    {
        _sock.disconnectFromHost ();
    }

    void send_raw (char* p, size_t len)
    {
        _sock.write (p, len);
    }

    size_t recv_raw (char* p, size_t len)
    {
        return _sock.read (p, len);
    }

    bool is_connected () const
    { return _sock.state () == QAbstractSocket::ConnectedState; }

    QString state () const;

protected slots:
    void socketStateChanged (QAbstractSocket::SocketState state);
};

#endif // __CONTROLLERCONNECTOR_H__
