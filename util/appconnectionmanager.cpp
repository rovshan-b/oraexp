#include "appconnectionmanager.h"
#include "connection_page/connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include "util/asyncdisconnect.h"

AppConnectionManager *AppConnectionManager::instance = 0;
QMutex AppConnectionManager::mutex;

AppConnectionManager::AppConnectionManager() : activeConnectionCount(0)
{
}

void AppConnectionManager::registerConnection(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab, DbConnection *db)
{
    QMutexLocker locker(&AppConnectionManager::mutex);

    Q_ASSERT(!AppConnectionManager::defaultInstance()->connectionList[cnPage][cnPageTab].contains(db));

    AppConnectionManager::defaultInstance()->connectionList[cnPage][cnPageTab].append(db);
    ++AppConnectionManager::defaultInstance()->activeConnectionCount;
}

void AppConnectionManager::deleteConnection(DbConnection *db)
{
    AppConnectionManager::unregisterConnection(db);

    AsyncDisconnect *asyncDisconnect = new AsyncDisconnect(db);
    connect(asyncDisconnect, SIGNAL(disconnected(DbConnection*)), AppConnectionManager::defaultInstance(), SLOT(disconnected(DbConnection*)));
    asyncDisconnect->start();
}

int AppConnectionManager::getActiveConnectionCount()
{
    return defaultInstance()->activeConnectionCount;
}

void AppConnectionManager::cleanup()
{
    delete AppConnectionManager::instance;
    AppConnectionManager::instance = 0;
}

AppConnectionManager *AppConnectionManager::defaultInstance()
{
    if(!AppConnectionManager::instance){
        AppConnectionManager::instance = new AppConnectionManager();
    }

    return AppConnectionManager::instance;
}

void AppConnectionManager::unregisterConnection(DbConnection *db)
{
    QMutexLocker locker(&AppConnectionManager::mutex);

    Q_ASSERT(AppConnectionManager::defaultInstance());

    bool removed = false;

    QHashIterator<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > i1(AppConnectionManager::defaultInstance()->connectionList);
    while (i1.hasNext()) {
        i1.next();

        //ConnectionPage* cnPage = i1.key();

        const QHash<ConnectionPageObject*, QList<DbConnection*> > &tabConnections = i1.value();

        QHashIterator<ConnectionPageObject*, QList<DbConnection*> > i2(tabConnections);

        while (i2.hasNext()) {
            i2.next();

            const QList<DbConnection*> &dbList = i2.value();

            int ix = dbList.indexOf(db);

            if(ix!=-1){
                AppConnectionManager::defaultInstance()->connectionList[i1.key()][i2.key()].removeOne(db);
                removed = true;
            }
        }
    }

    Q_ASSERT(removed);
}

void AppConnectionManager::disconnected(DbConnection *db)
{
    Q_UNUSED(db);

    --AppConnectionManager::defaultInstance()->activeConnectionCount;

    Q_ASSERT(AppConnectionManager::defaultInstance()->activeConnectionCount >= 0);

    emit connectionDisconnected(db);
}

QList<DbConnection *> AppConnectionManager::getAll()
{
    return AppConnectionManager::getConnections(0, 0);
}

QList<DbConnection *> AppConnectionManager::getByConnectionPage(ConnectionPage *cnPage)
{
    return AppConnectionManager::getConnections(cnPage, 0);
}

QList<DbConnection *> AppConnectionManager::getByConnectionPageObject(ConnectionPageObject *cnPageTab)
{
    return AppConnectionManager::getConnections(0, cnPageTab);
}

QList<DbConnection *> AppConnectionManager::getConnections(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab)
{
    Q_ASSERT(AppConnectionManager::defaultInstance());

    QList<DbConnection*> result;

    QHashIterator<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > i1(AppConnectionManager::defaultInstance()->connectionList);
    while (i1.hasNext()) {
        i1.next();

        //ConnectionPage* cnPage = i1.key();

        if(cnPage!=0 && i1.key() != cnPage){
            continue;
        }

        const QHash<ConnectionPageObject*, QList<DbConnection*> > &tabConnections = i1.value();

        QHashIterator<ConnectionPageObject*, QList<DbConnection*> > i2(tabConnections);

        while (i2.hasNext()) {
            i2.next();

            if(cnPageTab!=0 && i2.key() != cnPageTab){
                continue;
            }

            const QList<DbConnection*> &dbList = i2.value();

            foreach(DbConnection *db, dbList){
                result.append(db);
            }
        }
    }

    return result;
}
