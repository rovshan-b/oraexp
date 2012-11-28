#include "sequenceviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "util/iconutil.h"

SequenceViewerTabs::SequenceViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Sequence, uiManager, parent)
{
}

void SequenceViewerTabs::createTabs()
{
    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_sequence_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectReferencesViewer *referenceViewer=new DbObjectReferencesViewer(false, uiManager, this);
    addTab(referenceViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
