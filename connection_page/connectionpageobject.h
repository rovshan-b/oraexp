#ifndef CONNECTIONPAGEOBJECT_H
#define CONNECTIONPAGEOBJECT_H

#include "util/queryscheduler.h"
#include "interfaces/ideletelistener.h"

class DbUiManager;
class DbConnection;

class ConnectionPageObject : public QueryScheduler
{
public:
    ConnectionPageObject(DbUiManager *uiManager);
    virtual ~ConnectionPageObject();

    //this function can be used to create user interface if tab wants
    //to allow inherited classes to override ui creation logic
    virtual void createUi(){}

    bool needsSeparateConnection(){return  requiresSeparateConnection;}
    virtual void setConnection(DbConnection *db);

    virtual DbConnection *getDb() const {return this->db;}
    virtual void increaseRefCount();
    virtual void decreaseRefCount();

    bool isBusy() const;

    virtual void beforeClose() {}

    virtual bool isWindow() const {return false;}

    void setDeleteListener(IDeleteListener *deleteListener);

    virtual QString getDisplayName() const = 0;
    virtual QIcon getIcon() const = 0;

protected:
    DbUiManager *uiManager;
    DbConnection *db;
    bool requiresSeparateConnection;
    bool busy;

    int activeQueryCount;

    IDeleteListener *deleteListener;

    void beforeEnqueueQuery();

    void setBusy(bool busy);

    virtual void queryExecTaskCompleted(const QString &taskName) = 0;
    virtual void emitBusyStateChangedSignal() = 0;
    virtual void emitInitCompletedSignal() = 0;
};

#endif // CONNECTIONPAGEOBJECT_H
