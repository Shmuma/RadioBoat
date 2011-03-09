#include "ControllerConnector.hpp"

#include <QString>
#include <QByteArray>

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


void DirectConnector::send_raw (const QByteArray& data)
{
    int gone;
    libusb_bulk_transfer (_handle, 1 | LIBUSB_ENDPOINT_OUT, (unsigned char*)data.data (), data.count (), &gone, 1000);
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
    return tr ("Mode: remote");
}
