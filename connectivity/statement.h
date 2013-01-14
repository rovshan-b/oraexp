#ifndef STATEMENT_H
#define STATEMENT_H

#include <ocilib.h>
#include <QString>
#include "util/param.h"
#include "resultset.h"
#include "enums.h"
#include "connectivity/queryresult.h"
#include <ocilib.h>

class Statement
{
public:
    Statement(Connection *connection);
    Statement();
    ~Statement();

#ifdef DEBUG
    QString QUERY_FOR_DBG;
#endif

    void prepare(const QString &query);
    void bindParams(QList<Param*> parameters, bool takeOwnershipOfParams=true);
    void bindParam(Param *parameter);
    QueryResult execute();

    int rsCount() const;
    Resultset *rsAt(int index) const;
    //void addRs(Resultset *rs);

    OraExp::QueryType getStatementType() const {return this->statementType;}
    QString getStatementVerb() const {return this->verb;}
    int getAffectedRecordCount() const {return this->affectedRecordCount;}

    static void printObjectCount();

    bool removeResultset(Resultset *rs);
    OCI_Statement *ociStatement() const {return this->ociStmt;}
    Param *getParamByName(const QString &paramName);

    bool isPrepared(){return this->prepared;}

    int paramCount() const;
    Param *param(int i) const;
    bool isAnonymousBlock();

    void lockConnection();
    void unlockConnection();
    bool hasLockOnConnection() const {return this->acquiredMutex;}

    void *createDateArray(int size);
    void *createTimestampArray(const QString &timestampTypeName, int size);
    void *createIntervalArray(const QString &intervalTypeName, int size);

    void setBindArraySize(unsigned int size);
    void bindArrayOfStrings(const QString &bindVarName, dtext *data, int strLength, int plSqlTableLength);
    void bindArrayOfDoubles(const QString &bindVarName, double *data, int plSqlTableLength);
    void bindArrayOfDates(const QString &bindVarName, OCI_Date **data, int plSqlTableLength);
    void bindArrayOfTimestamps(const QString &bindVarName, OCI_Timestamp **data, int type, int plSqlTableLength);
    void bindArrayOfIntervals(const QString &bindVarName, OCI_Interval **data, int type, int plSqlTableLength);
    int setBindNullAtPos(int bindIx, int position);

    void printBindVars();

    void releaseResultsets();
private:
    void setConnection(Connection *connection);
    QueryResult doExecute();

    OCI_Statement *ociStmt;
    dtext *ociQuery;
    bool inDestructor;
    Connection *connection;

    QList<Param*> params;
    bool takeOwnershipOfParams;

    QList<Resultset*> resultsets;

    QList<dtext*> ociStrings;

    bool prepared;

    OraExp::QueryType statementType;
    QString verb;
    int affectedRecordCount;

    bool acquiredMutex;

    void init();
    void createOciStatement(Connection *connection);
    OraExp::QueryType determineStatementType();
    void cleanup();
    void freeOciStatement();
    void freeOciQuery();
    void checkForOciError();
    void collectParamResultsets();

    dtext *createOciString(const QString &str);

#ifdef DEBUG
    static QAtomicInt objectCount;
#endif

    void ref()
    {
#ifdef DEBUG
        Statement::objectCount.ref();
#endif
    }

    void deref()
    {
#ifdef DEBUG
        Statement::objectCount.deref();
#endif
    }

};

#endif // STATEMENT_H
