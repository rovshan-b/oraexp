#include "indexviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "../db_object_viewer/tabs/dbobjectpartitionsviewer.h"
#include "../db_object_viewer/tabs/dbobjectgrantsviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"

IndexViewerTabs::IndexViewerTabs(const QString &schemaName, const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Index, uiManager, parent)
{
}

void IndexViewerTabs::createTabs()
{
    DbObjectViewerGenericTab *columnInfo=new DbObjectViewerGenericTab("get_index_columns_for_detailed_view", uiManager);
    columnInfo->setIconColumn("INDEX_OWNER", "IO_ICON_COLUMN");
    columnInfo->setIconColumn("INDEX_NAME", "IND_ICON_COLUMN");
    columnInfo->setIconColumn("TABLE_OWNER", "TO_ICON_COLUMN");
    columnInfo->setIconColumn("TABLE_NAME", "TAB_ICON_COLUMN");
    columnInfo->setIconColumn("COLUMN_NAME", "COL_ICON_COLUMN");
    columnInfo->setObjectListMode(2,3,-1,QString(),"TABLE");
    addTab(columnInfo, IconUtil::getIcon("column"), tr("Columns"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_index_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectDetailsViewer *statisticsViewer = new DbObjectDetailsViewer("get_index_statistics_for_detailed_view", uiManager, this);
    addTab(statisticsViewer, IconUtil::getIcon("statistics"), tr("Statistics"));

    DbObjectPartitionsViewer *partitionInfo = new DbObjectPartitionsViewer(uiManager, this);
    addTab(partitionInfo, IconUtil::getIcon("partition"), tr("Partitions"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(false, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
