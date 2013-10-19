#ifndef CONNECTIONPAGEWINDOWOBJECT_H
#define CONNECTIONPAGEWINDOWOBJECT_H

#include "connectionpageobject.h"

class ConnectionPageWindowObject : public ConnectionPageObject
{
public:
    ConnectionPageWindowObject(DbUiManager *uiManager);

    virtual bool initiallyVisible() const{return true;}

    void showAndActivate();

    virtual bool isWindow() const {return true;}
};

#endif // CONNECTIONPAGEWINDOWOBJECT_H
