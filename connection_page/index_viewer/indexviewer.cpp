#include "indexviewer.h"
#include "indexviewertabs.h"

IndexViewer::IndexViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *IndexViewer::getViewerWidget() const
{
    return new IndexViewerTabs(schemaName, objectName, uiManager);
}
