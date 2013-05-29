#include "tableviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "../db_object_viewer/tabs/dbobjectpartitionsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "../db_object_viewer/tabs/dbobjectreferencesviewer.h"
#include "../db_object_viewer/tabs/dbobjectgrantsviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"
#include "beans/statementdesc.h"

TableViewerTabs::TableViewerTabs(const QString &schemaName, const QString &objectName,
                                 DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Table, uiManager, parent)
{
}

void TableViewerTabs::createTabs()
{
    DbObjectViewerGenericTab *columnInfo=new DbObjectViewerGenericTab("get_table_columns_for_detailed_view", uiManager);
    addTab(columnInfo, IconUtil::getIcon("column"), tr("Columns"));

    DbObjectDataViewer *dataViewer=new DbObjectDataViewer(uiManager, this);
    addTab(dataViewer, IconUtil::getIcon("data"), tr("Data"));

    DbObjectViewerGenericTab *constraintInfo = new DbObjectViewerGenericTab("get_table_constraints_for_detailed_view", uiManager, this);
    constraintInfo->setIconColumn("CONSTRAINT_NAME", "TYPE_ICON");
    constraintInfo->setObjectListMode(-1,0,-1,schemaName,"CONSTRAINT");
    addTab(constraintInfo, IconUtil::getIcon("constraint"), tr("Constraints"));

    DbObjectViewerGenericTab *indexInfo = new DbObjectViewerGenericTab("get_table_indexes_for_detailed_view", uiManager, this);
    indexInfo->setIconColumn("OWNER", "OWNER_ICON_COLUMN");
    indexInfo->setIconColumn("INDEX_NAME", "IND_ICON_COLUMN");
    indexInfo->setIconColumn("COLUMNS", "COL_ICON_COLUMN");
    indexInfo->setObjectListMode(0,1,-1,QString(),"INDEX");

    StatementDesc *desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_table_index_columns");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":object_name", objectName) <<
                 new Param(":index_owner", QString()) <<
                 new Param(":index_name", QString()) <<
                 new Param(":rs_out");
    desc->addReplacement(2, 1);
    desc->addReplacement(3, 2);

    indexInfo->setDynamicQuery(3, desc);
    addTab(indexInfo, IconUtil::getIcon("index"), tr("Indexes"));

    DbObjectViewerGenericTab *refFkInfo = new DbObjectViewerGenericTab("get_table_referencing_fks_for_detailed_view", uiManager, this);
    refFkInfo->setIconColumn("OWNER", "OWNER_ICON_COLUMN");
    refFkInfo->setIconColumn("TABLE_NAME", "TAB_ICON_COLUMN");
    refFkInfo->setIconColumn("CONSTRAINT_NAME", "FK_ICON_COLUMN");
    refFkInfo->setObjectListMode(0,1,-1,QString(),"TABLE");

    desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_constraint_column_names");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":constraint_name", QString());
    desc->addReplacement(1, 3);

    refFkInfo->setDynamicQuery(4, desc);
    addTab(refFkInfo, IconUtil::getIcon("constraint_fk"), tr("Referencing FKs"));

    DbObjectGrantsViewer *grantInfo = new DbObjectGrantsViewer(uiManager, this);
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Grants"));

    DbObjectTriggersViewer *triggerViewer = new DbObjectTriggersViewer(uiManager, this);
    addTab(triggerViewer, IconUtil::getIcon("trigger"), tr("Triggers"));

    DbObjectReferencesViewer *dependencyViewer=new DbObjectReferencesViewer(false, uiManager, this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_table_details_for_detailed_view", uiManager, this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    DbObjectPartitionsViewer *partitionInfo = new DbObjectPartitionsViewer(uiManager, this);
    addTab(partitionInfo, IconUtil::getIcon("partition"), tr("Partitions"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(true, uiManager, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
