#include "connectionpageobject.h"
#include "connectivity/dbconnection.h"
#include "util/appconnectionmanager.h"

ConnectionPageObject::ConnectionPageObject(DbUiManager *uiManager) :
    uiManager(uiManager),
    db(0),
    requiresSeparateConnection(true),
    busy(false),
    activeQueryCount(0),
    deleteListener(0)
{
}

ConnectionPageObject::~ConnectionPageObject()
{
    Q_ASSERT(deleteListener);
    deleteListener->beforeDelete(this);

    if(requiresSeparateConnection && db!=0){
        AppConnectionManager::deleteConnection(db);
        db = 0;
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
    if(db==0){
        return this->busy;
    }else{
        if(requiresSeparateConnection){
            return this->busy || db->isBusy();
        }else{
            return this->busy;
        }
    }
}

void ConnectionPageObject::setDeleteListener(IDeleteListener *deleteListener)
{
    this->deleteListener = deleteListener;
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
