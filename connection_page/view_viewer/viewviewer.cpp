#include "viewviewer.h"
#include "viewviewertabs.h"

ViewViewer::ViewViewer(const QString &schemaName,
                       const QString &objectName,
                       DbUiManager *uiManager,
                       QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *ViewViewer::getViewerWidget() const
{
    return new ViewViewerTabs(schemaName, objectName, uiManager);
}
