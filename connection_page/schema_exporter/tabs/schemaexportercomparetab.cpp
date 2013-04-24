#include "schemaexportercomparetab.h"

SchemaExporterCompareTab::SchemaExporterCompareTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerCompareTab(uiManager, parent)
{
}

bool SchemaExporterCompareTab::needsTargetConnection() const
{
    return false;
}
