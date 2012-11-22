#include "viewviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"
#include "beans/statementdesc.h"

ViewViewerTabs::ViewViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::View, uiManager, parent)
{
}

void ViewViewerTabs::createTabs()
{
    DbObjectViewerGenericTab *columnInfo=new DbObjectViewerGenericTab("get_view_columns_for_detailed_view", uiManager);
    addTab(columnInfo, IconUtil::getIcon("column"), tr("Columns"));

    DbObjectDataViewer *dataViewer=new DbObjectDataViewer(uiManager, this);
    addTab(dataViewer, IconUtil::getIcon("data"), tr("Data"));

    DbObjectViewerGenericTab *grantInfo = new DbObjectViewerGenericTab("get_table_grants_for_detailed_view", uiManager, this);
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Grants"));

    DbObjectTriggersViewer *triggerViewer = new DbObjectTriggersViewer(uiManager, this);
    addTab(triggerViewer, IconUtil::getIcon("trigger"), tr("Triggers"));

    DbObjectReferencesViewer *dependencyViewer=new DbObjectReferencesViewer(false, uiManager, this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectReferencesViewer *referenceViewer=new DbObjectReferencesViewer(true, uiManager, this);
    addTab(referenceViewer, IconUtil::getIcon("reference"), tr("References"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_view_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
