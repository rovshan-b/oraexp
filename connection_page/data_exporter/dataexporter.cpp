#include "dataexporter.h"
#include "tabs/dataexportercomparetab.h"
#include "tabs/dataexporteroptionstab.h"

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

void DataExporter::createTabs()
{
    DataComparer::createTabs();

    DataExporterCompareTab *cmpTab = static_cast<DataExporterCompareTab*>(compareTab);
    DataExporterOptionsTab *optTab = static_cast<DataExporterOptionsTab*>(optionsTab);
    optTab->getOptionsWidget()->createSeparatedUi(cmpTab->getTopPaneForm(), cmpTab->getBottomPaneTab());
}
