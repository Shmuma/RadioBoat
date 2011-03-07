#ifndef __CONTROLLERCONNECTOR_H__
#define __CONTROLLERCONNECTOR_H__

#include <QObject>
#include <QString>
#include <QByteArray>



// abstract connector with method
class ControllerConnector : public QObject
{
public:
    virtual bool connect (const QString& options) = 0;
    virtual void disconnect () = 0;

    virtual void send_raw (const QByteArray& data) = 0;

    virtual bool is_connected () const = 0;

    virtual QString state () const = 0;
signals:
    void connected ();
    void disconnected (const QString& reason);
    void state_changed (const QString& state);
};


class DirectConnector : public ControllerConnector
{
private:
    bool _connected;

public:
    DirectConnector ()
        : _connected (false)
    {};

    bool connect (const QString& options);
    void disconnect ();

    void send_raw (const QByteArray& data);

    bool is_connected () const
    { return _connected; };

    QString state () const;
};


class NetworkConnector : public ControllerConnector
{

};

#endif // __CONTROLLERCONNECTOR_H__
