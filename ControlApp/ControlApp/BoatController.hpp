#ifndef BOATCONTROLLER_HPP
#define BOATCONTROLLER_HPP

#include "ControllerConnector.hpp"


class BoatController : public QObject
{
    Q_OBJECT
private:
    ControllerConnector* _connector;

public:
    BoatController (QObject* parent)
        : QObject (parent),
          _connector (new DirectConnector ())
    {};

    ~BoatController ()
    {
        if (_connector) {
            delete _connector;
            _connector = NULL;
        }
    }

    const ControllerConnector* connector () const 
    { return _connector; };
};


#endif // BOATCONTROLLER_HPP
