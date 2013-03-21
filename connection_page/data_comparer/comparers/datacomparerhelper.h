#ifndef DATACOMPARERHELPER_H
#define DATACOMPARERHELPER_H

#include "interfaces/idbobjectcomparehelper.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/datacomparisonoptions.h"
#include "connectivity/ociexception.h"
#include "../tableinfofordatacomparison.h"
#include "navtree/dbtreemodel.h"
#include "connectivity/queryresult.h"
#include "beans/datacompareinfo.h"
#include <QQueue>

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

private:
    QString sourceSchema;
    IQueryScheduler *sourceScheduler;
    QString targetSchema;
    IQueryScheduler *targetScheduler;
    DbTreeModel *model;
    DataComparisonOptions *options;
    QHash<QString, TableInfoForDataComparison> tableOptions;

    QString currentTableName;

    QQueue<QModelIndex> itemsToCompare;

    DataComparerThread *comparerThread;

    QString tableNamesToCompare;

    QString lastExceptionTaskName;
    OciException lastException;

    QString enableRefContraintsQuery;

    void emitCompletedSignal();
    QStringList getItemsToCompare() const;
    void loadTableColumns();
    void compareNextItem();
    void startComparerThread(const QString &tableName);
    void deleteComparerThread();

    void startToCompare();

    void disableRefConstraints();
    void enableRefConstraints();
};

#endif // DATACOMPARERHELPER_H
