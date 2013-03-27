#ifndef DATACOPIERTHREAD_H
#define DATACOPIERTHREAD_H

#include "connection_page/data_operation/dataoperationthread.h"

class DataCopierOptions;

class DataCopierThread : public DataOperationThread
{
    Q_OBJECT
public:
    explicit DataCopierThread(const QString &sourceSchema,
                              DbConnection *sourceDb,
                              const QString &targetSchema,
                              DbConnection *targetDb,
                              const QString &tableName,
                              DataOperationOptions *options,
                              const TableInfoForDataComparison &tableOptions,
                              QObject *parent);

    virtual ~DataCopierThread();
    
protected:
    virtual void run();

private:
    DataCopierOptions *copyOptions;
    BulkOperationHelper bulkHelper;

    Statement *sourceStmt;
    Statement *targetStmt;

    virtual void prepareBindArrayForColumn(const QString &colName, const QString &dataType, int length, int colOffset);

    void copyData();
    
};

#endif // DATACOPIERTHREAD_H
