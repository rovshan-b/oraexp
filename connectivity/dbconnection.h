#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QString>
#include <QMetaType>
#include <QList>
#include <QSharedPointer>
#include "connection.h"
#include "util/param.h"
#include "util/queryqueuemanager.h"
#include "dblocalcache.h"
#include "queryrunner.h"

class Resultset;
class QueryQueueManager;

class DbConnection
{

friend QDataStream &operator<<(QDataStream &out, const DbConnection &myObj);
friend QDataStream &operator>>(QDataStream &in, DbConnection &myObj);

//friend class QueryRunner;

public:
    DbConnection();
    DbConnection(const QString title, const QString environment, const QString tnsName,
                 const QString username, const QString password, const bool savePassword);
    DbConnection(const DbConnection &other);

    ~DbConnection();

    static void initEnvironment();
    static void destroyEnvironment();

    void connect();
    void disconnect();

    QString getTitle() const;
    QString getEnvironment() const;
    QString getTnsName() const;
    QString getUsername() const;
    QString getPassword() const;
    void setPassword(const QString &password);
    bool getSavePassword() const;
    void setSavePassword(const bool savePassword);

    void setOciOwner(bool);
    bool isOciOwner();

    bool isBusy() const;
//    void setBusy(volatile bool busy);

    unsigned int getServerMajorVersion() const {return this->serverMajorVersion;}
    unsigned int getServerMinorVersion() const {return this->serverMinorVersion;}

    Statement *createStatement();
    QueryResult executeQuery(const QString &query, QList<Param*> parameters=QList<Param*>(), bool retrieveResultsetAsBindParameter=false);
    QueryResult executeNamedQuery(const QString &queryName, QList<Param*> parameters, bool retrieveResultsetAsBindParameter=false);
    void executeQueryAndCleanup(const QString &query, QList<Param*> parameters=QList<Param*>(), bool retrieveResultsetAsBindParameter=false);
    QString retrieveDbmsOutput();

    QString getSchemaName() const;

    int getMaxIdentifierLength();


    //DbLocalCache *getCacheManager();

    DbConnection *clone();

    QueryQueueManager *getQueueManager(){return queueManager;}
    void enqueueQuery(const QueryExecTask &task) {queueManager->enqueueQuery(task);}
    void enqueueQuery(const QString &queryName, QList<Param*> params,
                      QObject *requester, const QString &taskName,
                      const QString &queryCompletedSlotName,
                      const QString &fetchSlotName=QString(),
                      const QString &fetchCompletedSlotName=QString(),
                      bool retrieveResultsetAsBindParameter=false,
                      const QString &dbLinkName=QString(),
                      bool isUserQuery = false,
                      QObject *queryEndMonitor=0);

    bool supportsFlashbackArchive() const {return this->serverMajorVersion>=11;}
    bool supportsStorageMaxsizeClause() const {return this->serverMajorVersion>=11;}
    bool supportsVirtualColumns() const {return this->serverMajorVersion>=11;}
    bool supportsIntervalAndReferencePartitioning() const {return this->serverMajorVersion>=11;}
    bool supportsChangingLobRetention() const {return this->serverMajorVersion>=11;}
    bool supportsExternalTableProjectColumn() const {return this->serverMajorVersion>=10;}
    bool supportsHashPartitioningOnIndexes() const {return this->serverMajorVersion>=10;}
    bool supportsDbmsWrap() const {return this->serverMajorVersion>=10;}
    bool supportsTriggerOrdering() const {return this->serverMajorVersion>=11;}
    bool supportsCompileTimeWarnings() const {return true;}
    bool supportsNativeCompilation() const {return true;}
    bool supportsAlteringDatabaseLinks() const {return this->serverMajorVersion>=11;}
    bool supportsSchemaEditions() const {return this->serverMajorVersion>=11;}

    int compareVersion(unsigned int major, int minor);
    int compareVersion(DbConnection *other);
private:
    QString title;
    QString environment;
    QString tnsName;
    QString username;
    QString password;
    bool savePassword;

    Connection connection;

    QueryQueueManager *queueManager;

    //QSharedPointer<DbLocalCache> localCache;

    //void determineServerFeatures();

    unsigned int serverMajorVersion;
    unsigned int serverMinorVersion;

};

Q_DECLARE_METATYPE(DbConnection)

#endif // DBCONNECTION_H
