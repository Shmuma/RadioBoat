#include "BoatController.hpp"

#include <QByteArray>


BoatController::~BoatController()
{
    disconnect ();
    if (_connector)
        delete _connector;
}


bool BoatController::connect ()
{
    if (_connector->connect ()) {
        update (true);
        return true;
    }
    else
        return false;
}


void BoatController::disconnect ()
{
    update (false);
    _connector->disconnect();
}


void BoatController::setThrottle (int percent)
{
    _p_throttle = percent;
    update ();
}


void BoatController::setRotate (int percent)
{
    _p_rotate = percent;
    update ();
}


void BoatController::update (bool enabled)
{
    if (!_connector->is_connected())
        return;

    QByteArray data;
    data.append (enabled ? 1 : 0);    // enabled
    data.append (_p_throttle);
    data.append (_p_rotate);
    _connector->send_raw (data);
}

