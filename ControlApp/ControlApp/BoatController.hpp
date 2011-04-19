#ifndef BOATCONTROLLER_HPP
#define BOATCONTROLLER_HPP

#include "ControllerConnector.hpp"
#include "ControllerProtocol.hpp"
#include "options.hpp"

#include <QObject>

class BoatController : public QObject
{
    Q_OBJECT
private:
    ControllerConnector* _connector;
    ControllerProtocol* _proto;
    int _p_throttle, _p_rotate;

public:
    BoatController (const Options &opts, QObject* parent)
        : QObject (parent),
          _connector (0),
          _proto (0),
          _p_throttle (0),
          _p_rotate (50)
    {
        updateOptions (opts);
    };

    ~BoatController ();

    ControllerConnector* connector () const
    { return _connector; }

    void setThrottle (int percent);
    void setRotate (int percent);
    void update (bool enabled = true);

    bool connectDevice ();
    void disconnectDevice ();

    void updateOptions (const Options &opts);

protected slots:
    void _connectorStateChanged (const QString& state);

signals:
    void connectorStateChanged (const QString& state);
};


#endif // BOATCONTROLLER_HPP
