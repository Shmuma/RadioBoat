#include "ControllerConnector.hpp"

#include <QString>
#include <QByteArray>


// ------------------------------------------------------------
// DirectConnector
// ------------------------------------------------------------
bool DirectConnector::connect (const QString& options)
{
    return false;
}


void DirectConnector::disconnect ()
{

}


void DirectConnector::send_raw (const QByteArray& data)
{

}


QString DirectConnector::state () const
{
    if (!_connected)
        return QString (tr ("Mode: local, State: disconnected"));
    else
        return QString (tr ("Mode: local, State: connected"));
}
