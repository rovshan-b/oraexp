#ifndef DATACOMPARERTHREAD_H
#define DATACOMPARERTHREAD_H

#include <QThread>
#include "connection_page/data_operation/dataoperationthread.h"

class DataComparisonOptions;

class DataComparerThread : public DataOperationThread
{
    Q_OBJECT
public:
    explicit DataComparerThread(const QString &sourceSchema,
                                DbConnection *sourceDb,
                                const QString &targetSchema,
                                DbConnection *targetDb,
                                const QString &tableName,
                                DataOperationOptions *options,
                                const TableInfoForDataComparison &tableOptions,
                                QObject *parent);

    virtual ~DataComparerThread();

protected:
    virtual void run();

private:
    QString compareScript;
    QString reverseCompareScript;

    DataComparisonOptions *compareOptions;
    BulkOperationHelper bulkHelper;
    BulkOperationHelper bulkDeleteHelper;

    Statement *targetStmt;
    Statement *sourceDeleteGeneratorStmt;

    QStringList uqColumns;
    QList<int> uqColumnOffsets;

    QString getUqColumns();
    virtual void prepareBindArrayForColumn(const QString &colName, const QString &dataType, int length, int colOffset);
    void createComparisonScript();
    void doComparison();
    void doReverseComparison();

};

#endif // DATACOMPARERTHREAD_H
