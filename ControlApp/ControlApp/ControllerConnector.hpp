#ifndef __CONTROLLERCONNECTOR_H__
#define __CONTROLLERCONNECTOR_H__

#include <QObject>
#include <QString>
#include <QByteArray>

#include "libusb/libusb.h"


// abstract connector with method
class ControllerConnector : public QObject
{
    Q_OBJECT
public:
    virtual bool connect () = 0;
    virtual void disconnect () = 0;

    virtual void send_raw (const QByteArray& data) = 0;

    virtual bool is_connected () const = 0;

    virtual QString state () const = 0;
signals:
    void connected ();
    void disconnected (const QString& reason);
    void stateChanged (const QString& state);
};


class DirectConnector : public ControllerConnector
{
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

    void send_raw (const QByteArray& data);

    bool is_connected () const
    { return _connected; }

    QString state () const;
};


class NetworkConnector : public ControllerConnector
{
};

#endif // __CONTROLLERCONNECTOR_H__
