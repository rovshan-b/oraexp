#include "dblinkviewer.h"
#include "dblinkviewertabs.h"

DbLinkViewer::DbLinkViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *DbLinkViewer::getViewerWidget() const
{
    return new DbLinkViewerTabs(schemaName, objectName, uiManager);
}
