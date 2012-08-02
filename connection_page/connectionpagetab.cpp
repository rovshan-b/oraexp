#include "connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

#include <iostream>
using namespace std;

ConnectionPageTab::ConnectionPageTab(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent), uiManager(uiManager), db(0), requiresSeparateConnection(false), busy(true), activeQueryCount(0)
{
}

ConnectionPageTab::~ConnectionPageTab()
{
    qDeleteAll(children());
    if(requiresSeparateConnection && db!=0){
        delete db;
    }
}

void ConnectionPageTab::setConnection(DbConnection *db)
{
    this->db=db;
}

bool ConnectionPageTab::isBusy() const
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

void ConnectionPageTab::setBusy(bool busy)
{
    this->busy=busy;
    emit tabBusyStateChanged(this, busy);
}

void ConnectionPageTab::increaseRefCount()
{
    if(!this->busy){
        setBusy(true);
    }

    ++activeQueryCount;
}

void ConnectionPageTab::decreaseRefCount()
{
    --activeQueryCount;

    Q_ASSERT(activeQueryCount>=0);

    if(activeQueryCount==0){
        setBusy(false);
    }
}

void ConnectionPageTab::beforeEnqueueQuery()
{
    Q_ASSERT(db); 
    increaseRefCount();
}

void ConnectionPageTab::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}
