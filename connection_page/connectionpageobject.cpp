#include "connectionpageobject.h"
#include "connectivity/dbconnection.h"

ConnectionPageObject::ConnectionPageObject(DbUiManager *uiManager) :
    uiManager(uiManager),
    db(0),
    requiresSeparateConnection(true),
    busy(false),
    activeQueryCount(0)
{
}

ConnectionPageObject::~ConnectionPageObject()
{
    if(requiresSeparateConnection && db!=0){
        delete db;
    }
}

void ConnectionPageObject::setConnection(DbConnection *db)
{
    this->db=db;
}

void ConnectionPageObject::increaseRefCount()
{
    if(!this->busy){
        setBusy(true);
    }

    ++activeQueryCount;
}

void ConnectionPageObject::decreaseRefCount()
{
    --activeQueryCount;

    Q_ASSERT(activeQueryCount>=0);

    if(activeQueryCount==0){
        setBusy(false);
    }
}

bool ConnectionPageObject::isBusy() const
{
    if(db==NULL){
        return this->busy;
    }else{
        if(requiresSeparateConnection){
            return this->busy || db->isBusy();
        }else{
            return this->busy;
        }
    }
}

void ConnectionPageObject::beforeEnqueueQuery()
{
    Q_ASSERT(db);
    increaseRefCount();
}

void ConnectionPageObject::setBusy(bool busy)
{
    this->busy=busy;

    emitBusyStateChangedSignal();
}
