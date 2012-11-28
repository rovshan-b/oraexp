#include "userviewer.h"
#include "userviewertabs.h"

UserViewer::UserViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *UserViewer::getViewerWidget() const
{
    return new UserViewerTabs(schemaName, objectName, this->uiManager);
}
