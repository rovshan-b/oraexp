#include "synonymviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "../db_object_viewer/tabs/dbobjectgrantsviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"


SynonymViewerTabs::SynonymViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Synonym, uiManager, parent)
{
}

void SynonymViewerTabs::createTabs()
{
    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_synonym_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectReferencesViewer *dependencyViewer=new DbObjectReferencesViewer(false, uiManager, this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectReferencesViewer *referenceViewer=new DbObjectReferencesViewer(true, uiManager, this);
    addTab(referenceViewer, IconUtil::getIcon("reference"), tr("References"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
