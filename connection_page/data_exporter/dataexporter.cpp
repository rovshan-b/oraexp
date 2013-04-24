#include "dataexporter.h"
#include "tabs/dataexportercomparetab.h"
#include "tabs/dataexporteroptionstab.h"
#include "tabs/dataexporterresultstab.h"
#include "dataexporthelper.h"

DataExporter::DataExporter(DbUiManager *uiManager, QWidget *parent) :
    DataComparer(uiManager, parent)
{
}

DbObjectComparerCompareTab *DataExporter::doCreateCompareTab(DbUiManager *uiManager)
{
    return new DataExporterCompareTab(uiManager, this);
}

DbObjectComparerOptionsTab *DataExporter::doCreateOptionsTab()
{
    return new DataExporterOptionsTab(this);
}

DbObjectComparerResultsTab *DataExporter::doCreateResultsTab()
{
    return new DataExporterResultsTab(this->uiManager, this);
}

IDbObjectCompareHelper *DataExporter::createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler, const QString &targetSchema, IQueryScheduler *targetScheduler, DbObjectComparisonOptions *options, QObject *parent)
{
    DataExportHelper *exportHelper=new DataExportHelper(sourceSchema,
                                              sourceScheduler,
                                              targetSchema,
                                              targetScheduler,
                                              compareTab->getObjectsModel(),
                                              static_cast<DataOperationOptions*>(options),
                                              static_cast<DataComparerCompareTab*>(compareTab)->getTableOptions(),
                                              parent);

    connectComparerSignalsAndSlots(exportHelper);

    return exportHelper;
}

void DataExporter::createTabs()
{
    DataComparer::createTabs();

    DataExporterOptionsTab *optTab = static_cast<DataExporterOptionsTab*>(optionsTab);
    optTab->getOptionsWidget()->createSeparatedUi(compareTab->getTopPaneForm(), compareTab->getBottomPaneTab());
}
