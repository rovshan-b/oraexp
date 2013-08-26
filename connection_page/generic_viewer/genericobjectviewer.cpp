#include "genericobjectviewer.h"
#include "genericobjectviewertabs.h"
#include <QDebug>

GenericObjectViewer::GenericObjectViewer(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewer(schemaName, objectName, uiManager, parent)
{
}

DbObjectViewerTabs *GenericObjectViewer::getViewerWidget() const
{
    return new GenericObjectViewerTabs(schemaName, objectName, properties["DB_OBJECT_TYPE"], this->uiManager);
}
