#ifndef IDELETELISTENER_H
#define IDELETELISTENER_H

class ConnectionPageObject;

class IDeleteListener
{
public:
    virtual void beforeDelete(ConnectionPageObject *obj) = 0;
    virtual ~IDeleteListener() {}
};

#endif // IDELETELISTENER_H
