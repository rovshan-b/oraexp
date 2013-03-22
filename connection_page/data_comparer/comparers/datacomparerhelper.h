#ifndef DATACOMPARERHELPER_H
#define DATACOMPARERHELPER_H

#include "interfaces/idbobjectcomparehelper.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/datacomparisonoptions.h"
#include "connectivity/ociexception.h"
#include "../tableinfofordatacomparison.h"
#include "navtree/dbtreemodel.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/datacompareinfo.h"

class DbTreeModel;
class DataComparerThread;

class DataComparerHelper : public QObject, public IDbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit DataComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                  const QString &targetSchema, IQueryScheduler *targetScheduler,
                                DbTreeModel *model, DataComparisonOptions *options,
                                const QHash<QString, TableInfoForDataComparison> &tableOptions,
                                QObject *parent);
    virtual ~DataComparerHelper();

    virtual void compare();

signals:
    void statusChanged(const QString &newStatus);
    void completed();
    void comparisonError(const QString &taskName, const OciException &exception);
    void objectCountDetermined(int count);
    void chunkCompleted(int size);

    void compareInfoAvailable(const DataCompareInfo &info);

private slots:
    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);

    void tableComparisonCompleted();
    void tableComparisonError(const QString &taskName, const OciException &exception);

    void subComparisonError(const QString &taskName, const OciException &exception);

    void refConstraintsDisabled(const QueryResult &result);
    void refConstraintsEnabled(const QueryResult &result);

    void tableSortQueryCompleted(const QueryResult &result);
    void tableSortRecordFetched(const FetchResult &fetchResult);
    void tableSortFetchCompleted(const QString &);

private:
    QString sourceSchema;
    IQueryScheduler *sourceScheduler;
    QString targetSchema;
    IQueryScheduler *targetScheduler;
    DbTreeModel *model;
    DataComparisonOptions *options;
    QHash<QString, TableInfoForDataComparison> tableOptions;

    QString currentTableName;

    QList<QModelIndex> itemsToCompare;
    int currentItemIxToCompare;

    DataComparerThread *comparerThread;

    QString tableNamesToCompare;

    QString lastExceptionTaskName;
    OciException lastException;

    QString enableRefContraintsQuery;

    bool needToDisableRefConstraints;

    void emitCompletedSignal();
    QStringList getItemsToCompare() const;
    void sortTableNames(const QStringList &tableNames);
    void fillItemsToCompare(const QString &tables);
    void loadTableColumns();
    void compareNextItem();
    void startComparerThread(const QString &tableName);
    void deleteComparerThread();

    void startToCompare();

    void disableRefConstraints();
    void enableRefConstraints();
};

#endif // DATACOMPARERHELPER_H
