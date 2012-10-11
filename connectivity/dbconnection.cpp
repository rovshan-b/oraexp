#include "dbconnection.h"
#include "connectivity/statement.h"
#include "resultset.h"
#include "util/queryqueuemanager.h"
#include "util/queryutil.h"
#include "defines.h"
#include <memory>

using namespace std;


DbConnection::DbConnection() : queueManager(0)
{

}

DbConnection::DbConnection(const QString title, const QString environment, const QString tnsName,
                           const QString username, const QString password, const bool savePassword) :
                           title(title), environment(environment), tnsName(tnsName), username(username),
                           password(password), savePassword(savePassword), queueManager(0),
                           serverMajorVersion(0), serverMinorVersion(0)

{ 

}

DbConnection::DbConnection(const DbConnection &other)
{
    this->title=other.title;
    this->environment=other.environment;
    this->tnsName=other.tnsName;
    this->username=other.username;
    this->password=other.password;
    this->savePassword=other.savePassword;

    this->queueManager=0;
}

DbConnection::~DbConnection()
{
    disconnect();
}

void DbConnection::setOciOwner(bool newVal)
{
    connection.setOciOwner(newVal);
}

bool DbConnection::isOciOwner()
{
    return connection.isOciOwner();
}

bool DbConnection::isBusy() const
{
    if(queueManager==0){
        return connection.isBusy();
    }else{
        return (connection.isBusy() || queueManager->isBusy());
    }
}

//void DbConnection::setBusy(volatile bool busy)
//{
//    connection.setBusy(busy);
//}

QString DbConnection::getSchemaName() const
{
    return connection.getSchemaName();
}

void DbConnection::initEnvironment()
{
    Connection::initEnvironment();
}

void DbConnection::destroyEnvironment()
{
    Connection::destroyEnvironment();

    Statement::printObjectCount();
    Resultset::printObjectCount();
    Param::printObjectCount();
}

void DbConnection::connect()
{
    connection.disconnect();
    connection.connect(tnsName, username, password);

    //determineServerFeatures();
    serverMajorVersion=connection.getServerMajorVersion();
    serverMinorVersion=connection.getServerMinorVersion();

    queueManager=new QueryQueueManager(this);
}

void DbConnection::disconnect()
{
    connection.disconnect();

    delete queueManager;
    queueManager=0;
}

Statement *DbConnection::createStatement()
{
    return connection.createStatement();
}

QueryResult DbConnection::executeQuery(const QString &query, QList<Param*> parameters, bool retrieveResultsetAsBindParameter)
{
    if(retrieveResultsetAsBindParameter){
        parameters.append(new Param(":rs_out"));
    }

    return connection.executeQuery(query, parameters);
}

QueryResult DbConnection::executeNamedQuery(const QString &queryName, QList<Param*> parameters, bool retrieveResultsetAsBindParameter)
{
    return this->executeQuery(QueryUtil::getQuery(queryName, this), parameters, retrieveResultsetAsBindParameter);
}

void DbConnection::executeQueryAndCleanup(const QString &query, QList<Param*> parameters, bool retrieveResultsetAsBindParameter)
{
    QueryResult result = executeQuery(query, parameters, retrieveResultsetAsBindParameter);
    delete result.statement;
}

QString DbConnection::retrieveDbmsOutput()
{
    return connection.retrieveDbmsOutput();
}

int DbConnection::getMaxIdentifierLength()
{
    return MAX_IDENTIFIER_LENGTH;
}

/*DbLocalCache *DbConnection::getCacheManager()
{
    if(localCache.isNull()){
        localCache=QSharedPointer<DbLocalCache>(new DbLocalCache(this));
    }
    return localCache.data();
}*/

DbConnection *DbConnection::clone()
{
    DbConnection *newDb=new DbConnection(this->title, this->environment, this->tnsName, this->username, this->password, this->savePassword);
    return newDb;
}

void DbConnection::enqueueQuery(const QString &queryName, QList<Param*> params,
                  QObject *requester, const QString &taskName,
                  const QString &queryCompletedSlotName,
                  const QString &fetchSlotName,
                  const QString &fetchCompletedSlotName,
                  bool retrieveResultsetAsBindParameter,
                  const QString &dbLinkName,
                  bool isUserQuery,
                  QObject *queryEndMonitor)
{
    QueryExecTask task;
    if(queryName.startsWith('$')){ //queryName contains real query
        task.query=queryName.mid(1);
    }else{
        task.query=QueryUtil::getQuery(queryName, this);
    }
    task.params=params;
    task.requester=requester;
    task.taskName=taskName;
    task.queryCompletedSlotName=queryCompletedSlotName;
    task.fetchSlotName=fetchSlotName;
    task.fetchCompletedSlotName=fetchCompletedSlotName;
    task.retrieveResultsetAsBindParameter=retrieveResultsetAsBindParameter;
    task.queryEndMonitor=queryEndMonitor;
    task.dbLinkName=dbLinkName;
    task.isUserQuery=isUserQuery;

    //if(queryName=="create_object_hash_for_comparison"){
    //    cout << task.query.toStdString() << endl;
    //}

    queueManager->enqueueQuery(task);
}

int DbConnection::compareVersion(unsigned int major, int minor)
{
    if(serverMajorVersion==major && (serverMinorVersion==minor || minor==-1)){
        return 0;
    }else if(major>serverMajorVersion || (major==serverMajorVersion && minor>serverMinorVersion)){
        return -1;
    }else{
        return 1;
    }
}

int DbConnection::compareVersion(DbConnection *other)
{
    return compareVersion(other->serverMajorVersion, other->serverMinorVersion);
}

QString DbConnection::getTitle() const
{
    return title;
}

QString DbConnection::getEnvironment() const
{
    return environment;
}

QString DbConnection::getTnsName() const
{
    return tnsName;
}

QString DbConnection::getUsername() const
{
    return username;
}

QString DbConnection::getPassword() const
{
    return password;
}

void DbConnection::setPassword(const QString &password)
{
    this->password=password;
}

bool DbConnection::getSavePassword() const
{
    return savePassword;
}

void DbConnection::setSavePassword(const bool savePassword)
{
    this->savePassword=savePassword;
}

QDataStream &operator<<(QDataStream &out, const DbConnection &db)
{
    out << db.title << db.environment << db.tnsName << db.username << (db.savePassword ? db.password : QString("")) << db.savePassword;

    return out;
}

QDataStream &operator>>(QDataStream &in, DbConnection &db)
{
    in >> db.title >> db.environment >> db.tnsName >> db.username >> db.password >> db.savePassword;

    return in;
}
