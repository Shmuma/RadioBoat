#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <QString>

class Options
{
private:
    bool _localMode;

    // remote options
    QString _host;
    int _port;

public:
    Options ();

    void save ();

    bool localMode () const
    { return _localMode; }
    QString host () const
    { return _host; }
    int port () const
    { return _port; }

    void setLocalMode (bool val)
    { _localMode = val; }
    void setHost (const QString& host)
    { _host = host; }
    void setPort (int port)
    { _port = port; }
};

#endif // OPTIONS_HPP
