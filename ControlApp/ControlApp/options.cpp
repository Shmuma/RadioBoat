#include "options.hpp"

#include <QString>
#include <QSettings>


Options::Options ()
{
    QSettings sett;

    _localMode = sett.value ("localMode", true).toBool ();
    _host = sett.value ("host", "192.168.100.1").toString ();
    _port = sett.value ("port", 10201).toInt ();
}


void Options::save ()
{
    QSettings sett;

    sett.setValue ("localMode", _localMode);
    sett.setValue ("host", _host);
    sett.setValue ("port", _port);
}
