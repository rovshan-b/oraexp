#include <iostream>
#include "connection.h"
#include "statement.h"
#include "resultset.h"
#include "util/strutil.h"
#include "ociexception.h"
#include "util/queryutil.h"
#include "util/dbutil.h"
#include "defines.h"
#include <stdexcept>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>

#include <stdio.h>

using namespace std;


bool Connection::environmentInitialized=false;

void Connection::init()
{
    ociOwner=true;
    ociConnection=0;
    busy=false;
}

Connection::Connection()
{
    init();
}

Connection::Connection(const Connection &)
{
    init();
}

Connection::~Connection()
{
    disconnect();
}

void Connection::setOciOwner(bool newVal)
{
    ociOwner=newVal;
}

bool Connection::isOciOwner() const
{
    return ociOwner;
}

bool Connection::isBusy() const
{
    return this->busy;
}

void Connection::setBusy(volatile bool busy)
{
    Q_ASSERT(this->busy!=busy);

    this->busy=busy;
}

void Connection::lock()
{
    mutex.lock();
    setBusy(true);
}

void Connection::unlock()
{
    setBusy(false);
    mutex.unlock();
}

QString Connection::getSchemaName() const
{
    return this->dbSchemaName;
}

void err_handler(OCI_Error *err)
{
    wprintf(
                L"code  : ORA-%05i\n"
                L"msg   : %s\n"
                L"sql   : %s\n",
                OCI_ErrorGetOCICode(err),
                OCI_ErrorGetString(err),
                OCI_GetSql(OCI_ErrorGetStatement(err))
           );
}

void Connection::initEnvironment()
{
    if(!environmentInitialized){
        if (!OCI_Initialize(err_handler, NULL, OCI_ENV_THREADED | OCI_ENV_CONTEXT)){
            DbUtil::checkForOciError(0);
        }
        environmentInitialized=true;
        cout << "initialized environment" << endl;
    }
}

void Connection::destroyEnvironment()
{
    if(environmentInitialized){
        OCI_Cleanup();
        cout << "terminated environment" << endl;

        environmentInitialized=false;
    }
}

void Connection::connect(QString tns, QString username, QString password)
{
    disconnect();

    this->dbSchemaName=username.toUpper();

    dtext *ociTns=toOciString(tns);
    dtext *ociUsername=toOciString(username);
    dtext *ociPassword=toOciString(password);

    ociConnection=OCI_ConnectionCreate(ociTns, ociUsername, ociPassword, OCI_SESSION_DEFAULT);

    delete[] ociTns;
    delete[] ociUsername;
    delete[] ociPassword;

    if(ociConnection==NULL){
        DbUtil::checkForOciError(0);
    }

    cout << "connected" << endl;

    cout << "turning on dbms_output" << endl;
    OCI_ServerEnableOutput(ociConnection, 32000, 5, 255);

    qDebug() << toQString(OCI_GetVersionServer(ociConnection));
}

void Connection::disconnect()
{
    if(!ociOwner){
        return;
    }

    QMutexLocker locker(&mutex);

    if(ociConnection){
        OCI_ConnectionFree(ociConnection);
        ociConnection=NULL;
        cout << "terminated connection" << endl;
    }
}

unsigned int Connection::getServerMajorVersion() const
{
    return OCI_GetServerMajorVersion(this->ociConnection);
}

unsigned int Connection::getServerMinorVersion() const
{
    return OCI_GetServerMinorVersion(this->ociConnection);
}

Statement *Connection::createStatement()
{
    return new Statement(this);
}

QueryResult Connection::executeQuery(const QString &query,
                                     QList<Param*> parameters,
                                     bool scrollableResultsets)
{
    QMutexLocker mutexLocker(&mutex);

    if(this->isBusy()){
        throw OciException(QObject::tr("There is already an ongoing operation"), 0, 0, 0);
    }

    setBusy(true);

    QueryResult result;

    Statement *stmt=0;

    try{

        stmt=createStatement();
        stmt->setUseScrollableResultsets(scrollableResultsets);
        stmt->prepare(query);
        stmt->bindParams(parameters);
        result = stmt->execute();

    }catch(OciException &ex){
        delete stmt;
        result.statement=0;
        setBusy(false);
        throw ex;
    }

    setBusy(false);

    return result;
}

QString Connection::retrieveDbmsOutput()
{
    QString dbmsOutput;

    const dtext *p;
    while ((p = OCI_ServerGetOutput(ociConnection))!=NULL)
    {
        dbmsOutput.append(toQString(p)).append("\n");
    }

    return dbmsOutput;
}
