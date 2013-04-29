#include "schemaexportercomparetab.h"

SchemaExporterCompareTab::SchemaExporterCompareTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerCompareTab(uiManager, parent)
{
}

bool SchemaExporterCompareTab::needsTargetConnection() const
{
    return false;
}

DbTreeModel::DbTreeNodeTypes SchemaExporterCompareTab::getNodeTypesToDisplay() const
{
    DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay = DbObjectComparerCompareTab::getNodeTypesToDisplay();
    nodeTypesToDisplay.append(DbTreeModel::DatabaseLinks);
    return nodeTypesToDisplay;
}

DbTreeModel::DbTreeNodeTypes SchemaExporterCompareTab::getCheckableNodeTypes() const
{
    DbTreeModel::DbTreeNodeTypes checkableNodeTypes = DbObjectComparerCompareTab::getCheckableNodeTypes();
    checkableNodeTypes.append(DbTreeModel::DatabaseLink);
    return checkableNodeTypes;
}
