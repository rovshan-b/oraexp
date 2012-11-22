#include "tableviewer.h"
#include "tableviewertabs.h"

TableViewer::TableViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *TableViewer::getViewerWidget() const
{
    return new TableViewerTabs(schemaName, objectName, this->uiManager);
}
