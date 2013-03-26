#include "datacopier.h"
#include "tabs/datacopieroptionstab.h"
#include "tabs/datacopierresultstab.h"
#include "connection_page/data_comparer/tabs/datacomparercomparetab.h"
#include "datacopyhelper.h"

DataCopier::DataCopier(DbUiManager *uiManager, QWidget *parent) :
    DataComparer(uiManager, parent)
{
}

DbObjectComparerOptionsTab *DataCopier::doCreateOptionsTab()
{
    return new DataCopierOptionsTab();
}

DbObjectComparerResultsTab *DataCopier::doCreateResultsTab()
{
    return new DataCopierResultsTab(this->uiManager);
}

IDbObjectCompareHelper *DataCopier::createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler, const QString &targetSchema, IQueryScheduler *targetScheduler, DbObjectComparisonOptions *options, QObject *parent)
{
    DataCopyHelper *copier=new DataCopyHelper(sourceSchema,
                                              sourceScheduler,
                                              targetSchema,
                                              targetScheduler,
                                              compareTab->getObjectsModel(),
                                              static_cast<DataOperationOptions*>(options),
                                              static_cast<DataComparerCompareTab*>(compareTab)->getTableOptions(),
                                              parent);

    connectComparerSignalsAndSlots(copier);

    return copier;
}
