#ifndef APPCONNECTIONMANAGER_H
#define APPCONNECTIONMANAGER_H

#include <QHash>
#include <QMutex>

class ConnectionPage;
class ConnectionPageObject;
class DbConnection;

class AppConnectionManager : public QObject
{
    Q_OBJECT
public:
    static void registerConnection(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab, DbConnection *db);
    static void deleteConnection(DbConnection *db);

    static int getActiveConnectionCount();

    static void cleanup();

    static QList<DbConnection*> getAll();
    static QList<DbConnection*> getByConnectionPage(ConnectionPage *cnPage);
    static QList<DbConnection*> getByConnectionPageObject(ConnectionPageObject *cnPageTab);

    static AppConnectionManager *defaultInstance();

signals:
    void connectionDisconnected(DbConnection *db);

private slots:
    void disconnected(DbConnection *db);

private:
    AppConnectionManager();

    static QList<DbConnection*> getConnections(ConnectionPage *cnPage, ConnectionPageObject *cnPageTab);

    static void unregisterConnection(DbConnection *db);

    static QMutex mutex;

    static AppConnectionManager *instance;
    QHash<ConnectionPage*, QHash<ConnectionPageObject*, QList<DbConnection*> > > connectionList;
    int activeConnectionCount;
};

#endif // APPCONNECTIONMANAGER_H
