#ifndef DATACOMPARERTHREAD_H
#define DATACOMPARERTHREAD_H

#include <QThread>
#include "connectivity/ociexception.h"
#include "../tableinfofordatacomparison.h"
#include "connectivity/bulkoperationhelper.h"
#include "beans/datacompareinfo.h"
#include <QHash>

class DbConnection;
class DataComparisonOptions;

class DataComparerThread : public QThread
{
    Q_OBJECT
public:
    explicit DataComparerThread(const QString &sourceSchema,
                                DbConnection *sourceDb,
                                const QString &targetSchema,
                                DbConnection *targetDb,
                                const QString &tableName,
                                DataComparisonOptions *options,
                                const TableInfoForDataComparison &tableOptions,
                                QObject *parent);

    virtual ~DataComparerThread();

protected:
    virtual void run();

signals:
    void statusChanged(const QString &newStatus);
    void comparisonCompleted();
    void compareError(const QString &taskName, const OciException &ex);
    void compareInfoAvailable(const DataCompareInfo &info);

private:
    QString sourceSchema;
    DbConnection *sourceDb;
    QString targetSchema;
    DbConnection *targetDb;
    QString tableName;
    DataComparisonOptions *options;
    TableInfoForDataComparison tableOptions;
    QString compareScript;
    QString reverseCompareScript;

#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
    QHash<QString, QString> allColumns;
#endif

    BulkOperationHelper bulkHelper;
    BulkOperationHelper bulkDeleteHelper;

    Statement *targetStmt;
    Statement *sourceDeleteGeneratorStmt;

    QList<int> uqColumnOffsets;

    void emitCompletedSignal();
    QString getUqColumns();
    void prepareBindArrays(const QStringList &uqColumns);
    void createComparisonScript(const QStringList &uqColumns);
    QString toDynamicSqlValue(const QString &varName, const QString &dataType);
    void doComparison();
    void doReverseComparison(const QStringList &uqColumns);

    QString getColumnsForSelect(const QStringList &columnList);

    void emitCompareInfo(const QString &tableName, const QString &newStatus, int inserts=0, int updates=0, int deletes=0, const QString &dml="");

};

#endif // DATACOMPARERTHREAD_H
