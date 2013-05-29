#ifndef CONNECTIONPAGEWINDOWOBJECT_H
#define CONNECTIONPAGEWINDOWOBJECT_H

#include "connectionpageobject.h"

class ConnectionPageWindowObject : public ConnectionPageObject
{
public:
    ConnectionPageWindowObject(DbUiManager *uiManager);

    virtual bool initiallyVisible() const{return true;}
public slots:
    void showAndActivate();
};

#endif // CONNECTIONPAGEWINDOWOBJECT_H
