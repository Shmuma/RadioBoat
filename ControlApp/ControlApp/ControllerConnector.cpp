#include "ControllerConnector.hpp"

#include <QString>
#include <QByteArray>
#include <QMessageBox>

#include "libusb/libusb.h"

#define BOAT_VENDOR  0x04D8
#define BOAT_PRODUCT 0xD002


// ------------------------------------------------------------
// DirectConnector
// ------------------------------------------------------------
bool DirectConnector::connect ()
{
    if (_connected)
        return true;

    _handle = libusb_open_device_with_vid_pid (_ctx, BOAT_VENDOR, BOAT_PRODUCT);
    if (!_handle)
        return false;

    libusb_claim_interface (_handle, 0);
    _connected = true;
    emit connected ();
    emit stateChanged (state ());
    return true;
}


void DirectConnector::disconnect ()
{
    if (!_connected)
        return;
    libusb_close (_handle);
    _connected = false;
    emit disconnected (tr ("User request"));
    emit stateChanged (state ());
}


void DirectConnector::send_raw (char* p, size_t len)
{
    int gone, err;
    err = libusb_bulk_transfer (_handle, 1 | LIBUSB_ENDPOINT_OUT, reinterpret_cast<unsigned char*> (p), len, &gone, 1000);

    if (err != 0)
        QMessageBox::information (0, tr ("USB send error"), QString::number (err));
}


size_t DirectConnector::recv_raw (char* p, size_t len)
{
    int gone, err;

    err = libusb_bulk_transfer (_handle, 1 | LIBUSB_ENDPOINT_IN, reinterpret_cast<unsigned char*> (p), len, &gone, 1000);
    if (err == 0)
        return gone;
    return err;
}


QString DirectConnector::state () const
{
    if (!_connected)
        return tr ("Mode: local, State: disconnected");
    else
        return tr ("Mode: local, State: connected");
}


// ------------------------------------------------------------
// NetworkConnector
// ------------------------------------------------------------
QString NetworkConnector::state () const
{
    QString res = tr ("Mode: remote, State: ");
    switch (_sock.state ()) {
    case QAbstractSocket::UnconnectedState:
        return res + tr ("disconnected");
    case QAbstractSocket::HostLookupState:
        return res + tr ("lookup");
    case QAbstractSocket::ConnectingState:
        return res + tr ("connecting");
    case QAbstractSocket::ConnectedState:
        return res + tr ("connected");
    case QAbstractSocket::ClosingState:
        return res + tr ("closing");
    default:
        return res + tr ("unknown");
    }
    return res + tr ("unknown");
}


void NetworkConnector::socketStateChanged (QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState)
        emit disconnected ("");
    if (state == QAbstractSocket::ConnectedState)
        emit connected ();
    emit stateChanged (this->state ());
}
