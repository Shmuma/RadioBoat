#include "BoatController.hpp"

#include <QByteArray>
#include <QObject>
#include <QMessageBox>

BoatController::~BoatController()
{
    disconnect ();
    if (_connector)
        delete _connector;
}


bool BoatController::connectDevice ()
{
    if (_connector->connect ()) {
        QMessageBox::information (0, "Version", _proto->getVersion ());
        update (true);
        return true;
    }
    else
        return false;
}


void BoatController::disconnectDevice ()
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

    // QByteArray data;
    // data.append (enabled ? 1 : 0);    // enabled
    // data.append (_p_throttle);
    // data.append (_p_rotate);
    // _connector->send_raw (data);
}


void BoatController::updateOptions (const Options &opts)
{
    if (_connector)
        delete _connector;
    if (opts.localMode ()) {
        _connector = new DirectConnector;
        _proto = new DirectProtocol (_connector);
    }
    else {
        _connector = new NetworkConnector (opts.host (), opts.port ());
    }
    connect (_connector, SIGNAL (stateChanged (const QString&)), this, SLOT (_connectorStateChanged (const QString&)));
    emit connectorStateChanged (_connector->state ());
}


void BoatController::_connectorStateChanged (const QString& state)
{
    emit connectorStateChanged (state);
}
