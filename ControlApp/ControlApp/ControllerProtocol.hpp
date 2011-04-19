#ifndef __CONTROLLERPROTOCOL_H__
#define __CONTROLLERPROTOCOL_H__

#include <QString>

#include "ControllerConnector.hpp"

class ProtocolError
{
public:
    enum code_t {
        NotConnected,           // connector ot connected
        NoReply,                // no reply received
        ReplyMismatch,          // invalid reply kind
        ErrorReply,             // got non-zero reply
    };

private:
    code_t _code;
    int _reply;

public:
    ProtocolError (code_t code, int reply = 0)
        : _code (code),
          _reply (reply)
    {}

    code_t code () const
    { return _code; }
};


class ControllerProtocol
{
private:
    ControllerConnector *conn;

protected:
    ControllerConnector *getConn ()
    { return conn; };

public:
    ControllerProtocol (ControllerConnector* _conn)
        : conn (_conn)
    {};

    virtual QString getVersion () throw (ProtocolError) = 0;
    virtual void enableChannels (bool ch1, bool ch2) throw (ProtocolError) = 0;
    virtual void setChannelPulse (int channel, int deci_ms) throw (ProtocolError) = 0;
    virtual int  getChannelPulse (int channel) throw (ProtocolError) = 0;
};


class DirectProtocol : public ControllerProtocol
{
public:
    DirectProtocol (ControllerConnector* _conn)
        : ControllerProtocol (_conn)
    {};

    QString getVersion () throw (ProtocolError);
    void enableChannels (bool ch1, bool ch2) throw (ProtocolError);
    void setChannelPulse (int channel, int deci_ms) throw (ProtocolError);
    int  getChannelPulse (int channel) throw (ProtocolError);
};

#endif // __CONTROLLERPROTOCOL_H__
