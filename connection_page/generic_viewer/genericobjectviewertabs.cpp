#include "genericobjectviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "util/iconutil.h"

GenericObjectViewerTabs::GenericObjectViewerTabs(const QString &schemaName, const QString &objectName, const QString &dbObjectTypeName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Unknown, uiManager, parent),
    dbObjectTypeName(dbObjectTypeName)
{
}

void GenericObjectViewerTabs::createTabs()
{
    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    ddlViewer->setDbObjectTypeName(this->dbObjectTypeName);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
