#include "userviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectpartitionsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "../db_object_viewer/tabs/dbobjectgrantsviewer.h"
#include "util/iconutil.h"

UserViewerTabs::UserViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Schema, uiManager, parent)
{
}

void UserViewerTabs::createTabs()
{
    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_user_general_info_for_editing", uiManager, this);
    detailsViewer->setUseObjectNameParam(false);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectViewerGenericTab *roleInfo=new DbObjectViewerGenericTab("get_user_roles_for_detailed_view", uiManager);
    roleInfo->setUseObjectNameParam(false);
    roleInfo->setIconColumn("GRANTED_ROLE", "ICON_COLUMN");
    addTab(roleInfo, IconUtil::getIcon("users"), tr("Roles"));

    DbObjectViewerGenericTab *sysPrivInfo=new DbObjectViewerGenericTab("get_user_sys_privs_for_detailed_view", uiManager);
    sysPrivInfo->setUseObjectNameParam(false);
    sysPrivInfo->setIconColumn("PRIVILEGE", "ICON_COLUMN");
    addTab(sysPrivInfo, IconUtil::getIcon("grants"), tr("System privileges"));

    DbObjectViewerGenericTab *quotaInfo=new DbObjectViewerGenericTab("get_user_tablespace_quotas_as_dba_for_detailed_view", uiManager);
    quotaInfo->setUseObjectNameParam(false);
    quotaInfo->setIconColumn("TABLESPACE_NAME", "ICON_COLUMN");
    addTab(quotaInfo, IconUtil::getIcon("tablespace"), tr("Quotas"));

    DbObjectViewerGenericTab *grantInfo=new DbObjectViewerGenericTab("get_user_object_grants_for_detailed_view", uiManager);
    grantInfo->setUseObjectNameParam(false);
    grantInfo->setIconColumn("OWNER", "SCHEMA_ICON_COLUMN");
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Object privileges"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
