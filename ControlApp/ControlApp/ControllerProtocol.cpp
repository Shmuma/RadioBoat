#include <QString>

#include "ControllerProtocol.hpp"

// low-level control protocol structures
#include <Stage1/protocol.h>


// ------------------------------------------------------------
// DirectProtocol
// ------------------------------------------------------------
QString DirectProtocol::getVersion () throw (ProtocolError)
{
    if (!getConn ()->is_connected ())
        throw ProtocolError (ProtocolError::NotConnected);

    struct control_msg msg;
    struct reply_msg reply;

    msg.kind = MSG_GET_VER;
    getConn ()->send_raw (reinterpret_cast<char*>(&msg), sizeof (msg));
    if (!getConn ()->recv_raw (reinterpret_cast<char*>(&reply), sizeof (reply)))
        throw ProtocolError (ProtocolError::NoReply);
    if (reply.kind != MSG_GET_VER)
        throw ProtocolError (ProtocolError::ReplyMismatch);
    if (reply.u.reply != REPLY_OK)
        throw ProtocolError (ProtocolError::ErrorReply, reply.u.reply);

    return QString::number (reply.u.get_ver.major) + "." + QString::number (reply.u.get_ver.minor);
}


void DirectProtocol::enableChannels (bool ch1, bool ch2) throw (ProtocolError)
{

}


void DirectProtocol::setChannelPulse (int channel, int deci_ms) throw (ProtocolError)
{

}


int  DirectProtocol::getChannelPulse (int channel) throw (ProtocolError)
{
    return 0;
}
