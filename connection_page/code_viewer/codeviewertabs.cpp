#include "codeviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "../db_object_viewer/tabs/dbobjectgrantsviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"

CodeViewerTabs::CodeViewerTabs(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType,
                               DbUiManager *uiManager,
                               QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, itemType, uiManager, parent)
{
}

void CodeViewerTabs::createTabs()
{
    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));

    DbObjectGrantsViewer *grantInfo = new DbObjectGrantsViewer(uiManager, this);
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Grants"));

    DbObjectReferencesViewer *dependencyViewer=new DbObjectReferencesViewer(false, uiManager, this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectReferencesViewer *referenceViewer=new DbObjectReferencesViewer(true, uiManager, this);
    addTab(referenceViewer, IconUtil::getIcon("reference"), tr("References"));

    DbObjectViewerGenericTab *errorInfo = new DbObjectViewerGenericTab("get_code_errors", uiManager, this);
    errorInfo->setIconColumn("TEXT", "ICON_COLUMN");
    errorInfo->setUseObjectTypeParam();
    addTab(errorInfo, IconUtil::getIcon("error"), tr("Errors"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_code_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));
}
