#ifndef DATAEXPORTHELPER_H
#define DATAEXPORTHELPER_H

#include "connection_page/data_operation/dataoperationhelper.h"

class DataExportHelper : public DataOperationHelper
{
    Q_OBJECT
public:
    explicit DataExportHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                   const QString &targetSchema, IQueryScheduler *targetScheduler,
                   DbTreeModel *model, DataOperationOptions *options,
                   const QHash<QString, TableInfoForDataComparison> &tableOptions,
                              QObject *parent);

protected:
    virtual void startWorkerThread(const QString &tableName);

private slots:
    void recordsExported(int count);
    void tableDataExported();
    void exportError(const QString &errorMessage);
    
};

#endif // DATAEXPORTHELPER_H
