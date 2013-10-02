#ifndef CONNECTIONPAGEWINDOWOBJECT_H
#define CONNECTIONPAGEWINDOWOBJECT_H

#include "connectionpageobject.h"

class ConnectionPageWindowObject : public ConnectionPageObject
{
public:
    ConnectionPageWindowObject(DbUiManager *uiManager);
    virtual ~ConnectionPageWindowObject();

    virtual bool initiallyVisible() const{return true;}

    void showAndActivate();
};

#endif // CONNECTIONPAGEWINDOWOBJECT_H
