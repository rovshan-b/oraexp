#ifndef APPCONNECTIONMANAGER_H
#define APPCONNECTIONMANAGER_H

#include <QHash>
#include <QMutex>

class ConnectionPage;
class ConnectionPageObject;
class DbConnection;

class AppConnectionManager
{
public:
    static void registerConnection(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab, DbConnection *db);
    static void deleteConnection(DbConnection *db, bool unregister = true);

    static QList<DbConnection*> getAll();
    static QList<DbConnection*> getByConnectionPage(ConnectionPage *cnPage);
    static QList<DbConnection*> getByConnectionPageObject(ConnectionPageObject *cnPageTab);

private:
    AppConnectionManager();

    static QList<DbConnection*> getConnections(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab);

    static void unregisterConnection(DbConnection *db);

    static QHash<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > connectionList;

    static QMutex mutex;
};

#endif // APPCONNECTIONMANAGER_H
