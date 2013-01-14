#include "statement.h"
#include "connectivity/queryresult.h"
#include "connectivity/connection.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "ociexception.h"
#include <QDebug>

#ifdef DEBUG
QAtomicInt Statement::objectCount;
#endif

Statement::Statement(Connection *connection)
{
    init();

    createOciStatement(connection);

    ref();
}

Statement::Statement()
{
    ociStmt=0;
    connection=0;

    init();
    ref();
}

void Statement::init()
{
    takeOwnershipOfParams=true;
    ociQuery=0;
    inDestructor=false;
    prepared=false;
    statementType=OraExp::QueryTypeUnknown;
    affectedRecordCount=0;
    acquiredMutex=false;
}

Statement::~Statement()
{
    inDestructor=true;
    cleanup();

    if(acquiredMutex){
        qDebug("COULD DELETE Statement WITH MUTEX ACQUIRED. THIS IS A BUG.");
        connection->unlock();
    }

    //qDebug("deleted statement");

    deref();
}

void Statement::setConnection(Connection *connection)
{
    Q_ASSERT(this->connection==0);
    Q_ASSERT(ociStmt==0);
    createOciStatement(connection);
}

void Statement::createOciStatement(Connection *connection)
{
    this->connection=connection;
    ociStmt = OCI_StatementCreate(connection->getOciConnection());
    DbUtil::checkForOciError(ociStmt);
    OCI_AllowRebinding(ociStmt, true);
}

void Statement::prepare(const QString &query)
{
#ifdef DEBUG
    QUERY_FOR_DBG = query;
#endif

    Q_ASSERT(!prepared);
    prepared=true;

    Q_ASSERT(ociStmt);
    Q_ASSERT(ociQuery==0);
    ociQuery=toOciString(query);

    if(!OCI_Prepare(ociStmt, ociQuery)){
        DbUtil::checkForOciError(ociStmt);
    }
}

void Statement::bindParams(QList<Param*> parameters, bool takeOwnershipOfParams)
{
    if(parameters.size()==0){
        return;
    }
    this->takeOwnershipOfParams=takeOwnershipOfParams;

    Q_ASSERT(prepared);
    Q_ASSERT(ociStmt);
    Q_ASSERT(params.size()==0);

    params=parameters;

    for(int i=0; i<parameters.size(); i++){
        bindParam(parameters.at(i));
    }
}

void Statement::bindParam(Param *parameter)
{
    Param::ParamType paramType=parameter->getParamType();

    DateTime *paramDT;

    switch(paramType){
    case Param::Integer:
        OCI_BindInt(ociStmt,
                    parameter->getParamName().toStdWString().c_str(),
                    (int*)parameter->data);
        break;
    case Param::String:
        OCI_BindString(ociStmt,
                       parameter->getParamName().toStdWString().c_str(),
                       (dtext*)parameter->data,
                       dtslen((dtext*)parameter->data));
        break;
    case Param::Double:
        OCI_BindDouble(ociStmt,
                       parameter->getParamName().toStdWString().c_str(),
                       (double*)parameter->data);
        break;
    case Param::Datetime:
        paramDT=parameter->getDateTimeValue();
        paramDT->setConnection(this->connection);
        paramDT->copyToOci();
        OCI_BindDate(ociStmt,
                     parameter->getParamName().toStdWString().c_str(),
                     paramDT->ociDate());
        break;
    case Param::Stmt:
    {
        Statement *paramStmt=parameter->getStmtValue();
        paramStmt->setConnection(this->connection);
        OCI_BindStatement(ociStmt,
                          parameter->getParamName().toStdWString().c_str(),
                          paramStmt->ociStatement());
    }
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    DbUtil::checkForOciError(ociStmt);

    if(parameter->isNull()){
        OCI_BindSetNull(OCI_GetBind2(ociStmt, parameter->getParamName().toStdWString().c_str()));
    }

    DbUtil::checkForOciError(ociStmt);
}

QueryResult Statement::execute()
{
    try{
        return doExecute();
    }catch(OciException &ex){
        if(acquiredMutex){
            unlockConnection();
        }
        throw ex;
    }
}

QueryResult Statement::doExecute()
{
    Q_ASSERT(connection);
    if(connection->mutex.tryLock()){
        print("Statement::doExecute called with unlocked mutex. This is a bug. The program will exit.");
        exit(1);
    }

    Q_ASSERT(prepared);
    Q_ASSERT(ociStmt);
    int execResult=OCI_Execute(ociStmt);

    if(!execResult){
        DbUtil::checkForOciError(ociStmt);
    }

    QueryResult queryResult;

    if(this->verb.isEmpty()){
        this->verb=toQString(OCI_GetSQLVerb(ociStmt));
    }

    if(this->statementType==OraExp::QueryTypeUnknown){
        this->statementType=determineStatementType();
    }

    if(statementType==OraExp::QueryTypeUpdate || statementType==OraExp::QueryTypeDelete || statementType==OraExp::QueryTypeInsert){

        this->affectedRecordCount=OCI_GetAffectedRows(ociStmt);

    }else if(statementType==OraExp::QueryTypeBegin || statementType==OraExp::QueryTypeDeclare){

        collectParamResultsets();

    }

    DbUtil::checkForOciError(ociStmt);

    if(statementType==OraExp::QueryTypeSelect){
        OCI_Resultset *ociResultSet=OCI_GetResultset(ociStmt);
        if(ociResultSet==NULL){
            DbUtil::checkForOciError(ociStmt);
        }
        OCI_SetPrefetchSize(ociStmt, 20);
        OCI_SetFetchSize(ociStmt, 20);

        Q_ASSERT(this->resultsets.isEmpty());
        this->resultsets.append(new Resultset(ociResultSet, connection, this));
    }

    queryResult.statement=this;

    return queryResult;
}

OraExp::QueryType Statement::determineStatementType()
{
    OraExp::QueryType queryType;
    int stmtType = OCI_GetStatementType(ociStmt);

    switch(stmtType){
    case OCI_CST_SELECT:
        queryType=OraExp::QueryTypeSelect;
        break;
    case OCI_CST_UPDATE:
        queryType=OraExp::QueryTypeUpdate;
        break;
    case OCI_CST_DELETE:
        queryType=OraExp::QueryTypeDelete;
        break;
    case OCI_CST_INSERT:
        queryType=OraExp::QueryTypeInsert;
        break;
    case OCI_CST_CREATE:
        queryType=OraExp::QueryTypeCreate;
        break;
    case OCI_CST_DROP:
        queryType=OraExp::QueryTypeDrop;
        break;
    case OCI_CST_ALTER:
        queryType=OraExp::QueryTypeAlter;
        break;
    case OCI_CST_BEGIN:
        queryType=OraExp::QueryTypeBegin;
        break;
    case OCI_CST_DECLARE:
        queryType=OraExp::QueryTypeDeclare;
        break;
    default:
        queryType=OraExp::QueryTypeUnknown;
        break;
    }

    if(queryType==OraExp::QueryTypeUnknown && this->verb=="EXPLAIN"){
        queryType=OraExp::QueryTypeExplainPlan;
    }

    return queryType;
}

int Statement::rsCount() const
{
    return resultsets.size();
}

Resultset *Statement::rsAt(int index) const
{
    Q_ASSERT(resultsets.size()>index);

    return resultsets.at(index);
}

/*void Statement::addRs(Resultset *rs)
{
    resultsets.append(rs);
}*/

void Statement::printObjectCount()
{
#ifdef DEBUG
    qDebug() << "Total number of statements:" << (int)Statement::objectCount;
#endif
}

void Statement::freeOciStatement()
{
    if(ociStmt!=0){
        OCI_StatementFree(ociStmt);
        ociStmt=0;

        //qDebug("released oci statement");
    }
}

void Statement::freeOciQuery()
{
    if(ociQuery!=0){
        delete[] ociQuery;
        ociQuery=0;
    }
}

void Statement::cleanup()
{
    Q_ASSERT(inDestructor);

    freeOciStatement();

    releaseResultsets();

    if(takeOwnershipOfParams){
        qDeleteAll(params); //params may also contain Statement objects
    }

    freeOciQuery();

    for(int i=0; i<ociStrings.size(); ++i){
        delete[] ociStrings.at(i);
    }
}

bool Statement::removeResultset(Resultset *rs)
{
    if(!inDestructor){
        bool res = resultsets.removeOne(rs);
        Q_ASSERT(res);

        return true;
    }else{
        return false;
    }
}

Param *Statement::getParamByName(const QString &paramName)
{
    for(int i=0; i<params.size(); ++i){
        if(params.at(i)->getParamName()==paramName){
            return params.at(i);
        }
    }

    Q_ASSERT(false);

    return 0;
}

int Statement::paramCount() const
{
    return params.size();
}

Param *Statement::param(int i) const
{
    Q_ASSERT(i>=0 && i<params.size());

    return params.at(i);
}

void Statement::collectParamResultsets()
{   
    Param::ParamType paramType;
    for(int i=0; i<params.size(); i++){
        const Param *parameter=params.at(i);
        paramType=parameter->getParamType();

        if(paramType==Param::Stmt){
            Statement *childStmt=parameter->getStmtValue();
            OCI_Resultset *ociResultSet=OCI_GetResultset(childStmt->ociStatement());
            if(!ociResultSet){
                DbUtil::checkForOciError(ociStmt);
            }
            this->resultsets.append(new Resultset(ociResultSet, this->connection, this));
        }
    }
}

void Statement::releaseResultsets()
{
    qDeleteAll(resultsets);
}

bool Statement::isAnonymousBlock()
{
    return statementType==OraExp::QueryTypeBegin || statementType==OraExp::QueryTypeDeclare;
}

void Statement::lockConnection()
{
    acquiredMutex=true;
    connection->lock();
}

void Statement::unlockConnection()
{
    acquiredMutex=false;
    connection->unlock();
}

void *Statement::createDateArray(int size)
{
    void *result = OCI_DateArrayCreate(this->connection->getOciConnection(), size);
    if(!result){
        throw OciException("Failed to allocate date array");
    }

    return result;
}

void *Statement::createTimestampArray(const QString &timestampTypeName, int size)
{
    int timestampType=DbUtil::getTimestampType(timestampTypeName);
    void *result = OCI_TimestampArrayCreate(this->connection->getOciConnection(), timestampType, size);
    if(!result){
        DbUtil::checkForOciError();
    }

    return result;
}

void *Statement::createIntervalArray(const QString &intervalTypeName, int size)
{
    int intervalType=DbUtil::getIntervalType(intervalTypeName);
    void *result = OCI_IntervalArrayCreate(this->connection->getOciConnection(), intervalType, size);
    if(!result){
        DbUtil::checkForOciError();
    }

    return result;
}

void Statement::setBindArraySize(unsigned int size)
{
    OCI_BindArraySetSize(ociStmt, size);
    DbUtil::checkForOciError(ociStmt);
}

void Statement::bindArrayOfStrings(const QString &bindVarName, dtext *data, int strLength, int plSqlTableLength)
{
    OCI_BindArrayOfStrings(ociStmt, createOciString(bindVarName), data, strLength, plSqlTableLength);
    DbUtil::checkForOciError(ociStmt);
}

void Statement::bindArrayOfDoubles(const QString &bindVarName, double *data, int plSqlTableLength)
{
    OCI_BindArrayOfDoubles(ociStmt, createOciString(bindVarName), data, plSqlTableLength);
    DbUtil::checkForOciError(ociStmt);
}

void Statement::bindArrayOfDates(const QString &bindVarName, OCI_Date **data, int plSqlTableLength)
{
    OCI_BindArrayOfDates(ociStmt, createOciString(bindVarName), data, plSqlTableLength);
    DbUtil::checkForOciError(ociStmt);
}

void Statement::bindArrayOfTimestamps(const QString &bindVarName, OCI_Timestamp **data, int type, int plSqlTableLength)
{
    OCI_BindArrayOfTimestamps(ociStmt, createOciString(bindVarName), data, type, plSqlTableLength);
    DbUtil::checkForOciError(ociStmt);
}

void Statement::bindArrayOfIntervals(const QString &bindVarName, OCI_Interval **data, int type, int plSqlTableLength)
{
    OCI_BindArrayOfIntervals(ociStmt, createOciString(bindVarName), data, type, plSqlTableLength);
    DbUtil::checkForOciError(ociStmt);
}

dtext *Statement::createOciString(const QString &str)
{
    dtext *result = toOciString(str);
    ociStrings.append(result);

    return result;
}

int Statement::setBindNullAtPos(int bindIx, int position) //position starts at 1.
{
    return OCI_BindSetNullAtPos(OCI_GetBind(ociStmt, bindIx), position);
}

void Statement::printBindVars()
{
    int bindCount=OCI_GetBindCount(ociStmt);
    for(int i=1; i<=bindCount; ++i){
        OCI_Bind *bind=OCI_GetBind(ociStmt, i);
        qDebug() << "Bind var name =" << OCI_BindGetName(bind) <<
                    ", type =" << OCI_BindGetType(bind) <<
                    ", data count =" << OCI_BindGetDataCount(bind);
    }
}
