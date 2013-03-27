#ifndef DATACOPYHELPER_H
#define DATACOPYHELPER_H

#include "connection_page/data_operation/dataoperationhelper.h"

class DataCopyHelper : public DataOperationHelper
{
public:
    DataCopyHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                   const QString &targetSchema, IQueryScheduler *targetScheduler,
                   DbTreeModel *model, DataOperationOptions *options,
                   const QHash<QString, TableInfoForDataComparison> &tableOptions,
                   QObject *parent);

protected:
    virtual void startWorkerThread(const QString &tableName);
};

#endif // DATACOPYHELPER_H
