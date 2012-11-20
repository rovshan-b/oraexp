#include "codeviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
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
    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));

    DbObjectViewerGenericTab *grantInfo = new DbObjectViewerGenericTab("get_table_grants_for_detailed_view", this);
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Grants"));

    DbObjectReferencesViewer *dependencyViewer=new DbObjectReferencesViewer(false, this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectReferencesViewer *referenceViewer=new DbObjectReferencesViewer(true, this);
    addTab(referenceViewer, IconUtil::getIcon("dependency"), tr("References"));

    DbObjectViewerGenericTab *errorInfo = new DbObjectViewerGenericTab("get_code_errors", this);
    errorInfo->setUseObjectTypeParam();
    addTab(errorInfo, IconUtil::getIcon("error"), tr("Errors"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_code_details_for_detailed_view", this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));
}
