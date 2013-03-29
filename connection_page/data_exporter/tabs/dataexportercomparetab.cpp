#include "dataexportercomparetab.h"

DataExporterCompareTab::DataExporterCompareTab(DbUiManager *uiManager, QWidget *parent) :
    DataComparerCompareTab(uiManager, parent)
{
}

bool DataExporterCompareTab::needsTargetConnection() const
{
    return false;
}
