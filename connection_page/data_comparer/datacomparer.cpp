#include "datacomparer.h"
#include "tabs/datacomparercomparetab.h"
#include "tabs/datacomparerresultstab.h"
#include "tabs/datacompareroptionstab.h"
#include "comparers/datacomparerhelper.h"
#include "beans/datacomparisonoptions.h"
#include "navtree/dbtreeitem.h"

DataComparer::DataComparer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparer(uiManager, parent)
{
}

DbObjectComparerCompareTab *DataComparer::doCreateCompareTab(DbUiManager *uiManager)
{
    return new DataComparerCompareTab(uiManager, this);
}

DbObjectComparerOptionsTab *DataComparer::doCreateOptionsTab()
{
    return new DataComparerOptionsTab();
}

DbObjectComparerResultsTab *DataComparer::doCreateResultsTab()
{
    return new DataComparerResultsTab(this->uiManager);
}

IDbObjectCompareHelper *DataComparer::createCompareHelper(const QString &sourceSchema,
                                                          IQueryScheduler *sourceScheduler,
                                                          const QString &targetSchema,
                                                          IQueryScheduler *targetScheduler,
                                                          DbObjectComparisonOptions *options,
                                                          QObject *parent)
{
    DataComparerHelper *comparer=new DataComparerHelper(sourceSchema,
                                                            sourceScheduler,
                                                            targetSchema,
                                                            targetScheduler,
                                                            compareTab->getObjectsModel(),
                                                            static_cast<DataComparisonOptions*>(options),
                                                            static_cast<DataComparerCompareTab*>(compareTab)->getTableOptions(),
                                                            parent);

    connectComparerSignalsAndSlots(comparer);

    return comparer;
}

void DataComparer::connectComparerSignalsAndSlots(QObject *comparer)
{
    connect(comparer, SIGNAL(statusChanged(QString)), this, SLOT(statusChanged(QString)));
    connect(comparer, SIGNAL(compareInfoAvailable(DataCompareInfo)), resultsTab, SLOT(compareInfoAvailable(DataCompareInfo)));
    connect(comparer, SIGNAL(queryTextAvailable(QString)), this, SLOT(comparisonResultAvailable(QString)));
    connect(comparer, SIGNAL(completed()), this, SLOT(completed()));
    connect(comparer, SIGNAL(comparisonError(QString,OciException)), this, SLOT(comparisonError(QString,OciException)));
    connect(comparer, SIGNAL(objectCountDetermined(int)), this, SLOT(objectCountDetermined(int)));
    connect(comparer, SIGNAL(chunkCompleted(int)), this, SLOT(chunkCompleted(int)));
    connect(comparer, SIGNAL(tableCompareSuccess(QString)), this, SLOT(tableCompareSuccess(QString)));

}

void DataComparer::tableCompareSuccess(const QString &tableName)
{
    DataOperationOptions *options = static_cast<DataOperationOptions*>(optionsTab->getOptions());
    bool uncheck = options->uncheckInGuiOnSuccess;

    delete options;

    if(!uncheck){
        return;
    }

    DbTreeModel *model=compareTab->getObjectsModel();
    QModelIndex parentIndex = model->getChildIndex(QModelIndex(), DbTreeModel::Tables);
    Q_ASSERT(parentIndex.isValid());

    QModelIndex tableIndex = model->findByName(parentIndex, tableName);
    if(!tableIndex.isValid()){
        return;
    }
    DbTreeItem *item = static_cast<DbTreeItem*>(tableIndex.internalPointer());
    if(item->checkState()==Qt::Checked){
        model->checkItem(tableIndex, Qt::Unchecked, true);
    }
}
