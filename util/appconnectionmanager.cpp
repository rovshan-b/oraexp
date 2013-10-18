#include "appconnectionmanager.h"
#include "connection_page/connectionpagetab.h"
#include "connectivity/dbconnection.h"

QHash<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > AppConnectionManager::connectionList;
QMutex AppConnectionManager::mutex;

AppConnectionManager::AppConnectionManager()
{
}

void AppConnectionManager::registerConnection(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab, DbConnection *db)
{
    QMutexLocker locker(&AppConnectionManager::mutex);

    Q_ASSERT(!AppConnectionManager::connectionList[cnPage][cnPageTab].contains(db));

    AppConnectionManager::connectionList[cnPage][cnPageTab].append(db);
}

void AppConnectionManager::deleteConnection(DbConnection *db, bool unregister)
{
    if(unregister){
        AppConnectionManager::unregisterConnection(db);
    }

    delete db;
}

void AppConnectionManager::unregisterConnection(DbConnection *db)
{
    QMutexLocker locker(&AppConnectionManager::mutex);

    bool removed = false;

    QHashIterator<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > i1(AppConnectionManager::connectionList);
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
                AppConnectionManager::connectionList[i1.key()][i2.key()].removeOne(db);
                removed = true;
            }
        }
    }

    Q_ASSERT(removed);
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
    QList<DbConnection*> result;

    QHashIterator<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > i1(AppConnectionManager::connectionList);
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
