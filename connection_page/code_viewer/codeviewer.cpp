#include "codeviewer.h"
#include "codeviewertabs.h"

CodeViewer::CodeViewer(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
    this->itemType=itemType;
}

DbObjectViewerTabs *CodeViewer::getTabsWidget() const
{
    return new CodeViewerTabs(schemaName, objectName, itemType, uiManager);
}
