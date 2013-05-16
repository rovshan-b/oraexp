#ifndef DATAOPERATIONTHREAD_H
#define DATAOPERATIONTHREAD_H

#include "util/stopablethread.h"
#include "connectivity/ociexception.h"
#include "beans/tableinfofordatacomparison.h"
#include "connectivity/bulkoperationhelper.h"
#include "beans/datacompareinfo.h"

#include <QHash>

class DbConnection;
class DataOperationOptions;

class DataOperationThread : public StopableThread
{
    Q_OBJECT
public:
    explicit DataOperationThread(const QString &sourceSchema,
                                 DbConnection *sourceDb,
                                 const QString &targetSchema,
                                 DbConnection *targetDb,
                                 const QString &tableName,
                                 DataOperationOptions *options,
                                 const TableInfoForDataComparison &tableOptions,
                                 QObject *parent);

    virtual ~DataOperationThread() {}

signals:
    void statusChanged(const QString &newStatus);
    void compareInfoAvailable(const DataCompareInfo &info);
    void comparisonCompleted();
    void compareError(const QString &taskName, const OciException &ex);

protected:
    QString sourceSchema;
    DbConnection *sourceDb;
    QString targetSchema;
    DbConnection *targetDb;
    QString tableName;
    DataOperationOptions *options;
    TableInfoForDataComparison tableOptions;

    QHash<QString, QString> allColumns;

    void prepareBindArrays(bool datesAsVarchar = false);
    virtual void prepareBindArrayForColumn(const QString &colName, const QString &dataType, int length, int colOffset) = 0;
    QString toDynamicSqlValue(const QString &varName, const QString &dataType);
    QString getColumnsForSelect(const QStringList &columnList);

    void emitCompletedSignal();
    void emitCompareInfo(const QString &tableName, const QString &newStatus, int inserts=0, int updates=0, int deletes=0, const QString &dml="");
};

#endif // DATAOPERATIONTHREAD_H
