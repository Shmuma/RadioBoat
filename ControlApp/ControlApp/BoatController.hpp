#ifndef BOATCONTROLLER_HPP
#define BOATCONTROLLER_HPP

#include "ControllerConnector.hpp"


class BoatController : public QObject
{
    Q_OBJECT
private:
    ControllerConnector* _connector;
    int _p_throttle, _p_rotate;

public:
    BoatController (QObject* parent)
        : QObject (parent),
          _connector (new DirectConnector ()),
          _p_throttle (0),
          _p_rotate (50)
    {};

    ~BoatController ();

    ControllerConnector* connector () const
    { return _connector; }

    void setThrottle (int percent);
    void setRotate (int percent);
    void update (bool enabled = true);

    bool connect ();
    void disconnect ();
};


#endif // BOATCONTROLLER_HPP
