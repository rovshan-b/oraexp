#ifndef DATAOPERATIONTHREAD_H
#define DATAOPERATIONTHREAD_H


#include "connectivity/ociexception.h"
#include "tableinfofordatacomparison.h"
#include "connectivity/bulkoperationhelper.h"
#include "beans/datacompareinfo.h"

#include <QThread>
#include <QHash>

class DbConnection;
class DataOperationOptions;

class DataOperationThread : public QThread
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

    void stop() {this->stopped=true;}
    
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

#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
    QHash<QString, QString> allColumns;
#endif
    void prepareBindArrays();
    virtual void prepareBindArrayForColumn(const QString &colName, const QString &dataType, int length, int colOffset) = 0;
    QString toDynamicSqlValue(const QString &varName, const QString &dataType);
    QString getColumnsForSelect(const QStringList &columnList);

    void emitCompletedSignal();
    void emitCompareInfo(const QString &tableName, const QString &newStatus, int inserts=0, int updates=0, int deletes=0, const QString &dml="");

    bool stopped;
};

#endif // DATAOPERATIONTHREAD_H
