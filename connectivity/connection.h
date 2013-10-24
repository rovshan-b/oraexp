#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>
#include <QList>
#include <QMutex>
#include <iostream>
#include "util/param.h"
#include <ocilib.h>
#include "queryresult.h"

class Resultset;
struct OCI_Connection;
struct OCI_Statement;

class Connection
{
public:
    Connection();
    Connection(const Connection &);
    ~Connection();

    static void initEnvironment();
    static void destroyEnvironment();

    void connect(QString tns, QString username, QString password, OraExp::ConnectAs connectAs);
    //void reconnect(QString tns, QString username, QString password, OraExp::ConnectAs connectAs);
    void disconnect();

    OCI_Connection *getOciConnection(){return this->ociConnection;}

    bool isBusy() const;

    void lock();
    void unlock();

    unsigned int getServerMajorVersion() const;
    unsigned int getServerMinorVersion() const;

    Statement *createStatement();
    QueryResult executeQuery(const QString &query,
                             QList<Param *> parameters=QList<Param*>(),
                             bool scrollableResultsets=false);

    void breakCurrentQuery();

    QString getSchemaName() const;

    static void checkForOciError(OCI_Statement *ociStatement, OCI_Statement *ociChildStatement=0);

    QMutex mutex; //must be kept non-recursive
    QString retrieveDbmsOutput();
private:
    void init();

    void doDisconnect();

    void setBusy(volatile bool busy);

    static bool environmentInitialized;
    OCI_Connection *ociConnection;

    QString dbSchemaName;

    volatile bool busy;
};

#endif // CONNECTION_H
