#include "datacomparer.h"
#include "tabs/datacomparercomparetab.h"
#include "tabs/datacomparerresultstab.h"
#include "tabs/datacompareroptionstab.h"
#include "comparers/datacomparerhelper.h"
#include "beans/datacomparisonoptions.h"

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

    //connect(comparer, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), resultsTab, SLOT(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));
    connect(comparer, SIGNAL(statusChanged(QString)), this, SLOT(statusChanged(QString)));
    connect(comparer, SIGNAL(compareInfoAvailable(DataCompareInfo)), resultsTab, SLOT(compareInfoAvailable(DataCompareInfo)));
    connect(comparer, SIGNAL(completed()), this, SLOT(completed()));
    connect(comparer, SIGNAL(comparisonError(QString,OciException)), this, SLOT(comparisonError(QString,OciException)));
    connect(comparer, SIGNAL(objectCountDetermined(int)), this, SLOT(objectCountDetermined(int)));
    connect(comparer, SIGNAL(chunkCompleted(int)), this, SLOT(chunkCompleted(int)));

    return comparer;
}
