#include "dblinkviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "util/iconutil.h"

DbLinkViewerTabs::DbLinkViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::DatabaseLink, uiManager, parent)
{
}

void DbLinkViewerTabs::createTabs()
{
    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_dblink_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
