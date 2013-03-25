#ifndef DATACOMPARERHELPER_H
#define DATACOMPARERHELPER_H

#include "../dataoperationhelper.h"

class DataComparerThread;

class DataComparerHelper : public DataOperationHelper
{
    Q_OBJECT
public:
    explicit DataComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                  const QString &targetSchema, IQueryScheduler *targetScheduler,
                                DbTreeModel *model, DataOperationOptions *options,
                                const QHash<QString, TableInfoForDataComparison> &tableOptions,
                                QObject *parent);

    virtual bool canStart();

signals:
    void compareInfoAvailable(const DataCompareInfo &info);

protected:
    virtual void startWorkerThread(const QString &tableName);

    virtual void subComparisonErrorOccured(const QString &, const OciException &);
};

#endif // DATACOMPARERHELPER_H
