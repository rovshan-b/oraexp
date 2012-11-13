#include "tableviewertabs.h"
#include "../db_object_viewer/tabs/dbobjectdataviewer.h"
#include "../db_object_viewer/tabs/dbobjecttriggersviewer.h"
#include "../db_object_viewer/tabs/dbobjectdetailsviewer.h"
#include "tabs/tablepartitionsviewer.h"
#include "../db_object_viewer/tabs/dbobjectddlviewer.h"
#include "util/iconutil.h"
#include "util/queryutil.h"
#include "beans/statementdesc.h"
#include <iostream>

TableViewerTabs::TableViewerTabs(const QString &schemaName, const QString &objectName,
                                 DbUiManager *uiManager, QWidget *parent) :
    DbObjectViewerTabs(schemaName, objectName, DbTreeModel::Table, uiManager, parent)
{
}

void TableViewerTabs::createTabs()
{
    DbObjectViewerGenericTab *columnInfo=new DbObjectViewerGenericTab("get_table_columns_for_detailed_view");
    addTab(columnInfo, IconUtil::getIcon("column"), tr("Columns"));

    DbObjectDataViewer *dataViewer=new DbObjectDataViewer(this);
    addTab(dataViewer, IconUtil::getIcon("data"), tr("Data"));

    DbObjectViewerGenericTab *constraintInfo = new DbObjectViewerGenericTab("get_table_constraints_for_detailed_view", this);
    constraintInfo->setIconColumn("CONSTRAINT_NAME", "TYPE_ICON");
    addTab(constraintInfo, IconUtil::getIcon("constraint"), tr("Constraints"));

    DbObjectViewerGenericTab *refFkInfo = new DbObjectViewerGenericTab("get_table_referencing_fks_for_detailed_view", this);

    StatementDesc *desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_constraint_column_names");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":constraint_name", QString());
    desc->addReplacement(1, 3);

    refFkInfo->setDynamicQuery(4, desc);
    addTab(refFkInfo, IconUtil::getIcon("constraint_fk"), tr("Referencing FKs"));

    DbObjectViewerGenericTab *grantInfo = new DbObjectViewerGenericTab("get_table_grants_for_detailed_view", this);
    addTab(grantInfo, IconUtil::getIcon("grants"), tr("Grants"));

    DbObjectTriggersViewer *triggerViewer = new DbObjectTriggersViewer(this);
    addTab(triggerViewer, IconUtil::getIcon("trigger"), tr("Triggers"));

    DbObjectViewerGenericTab *dependencyViewer=new DbObjectViewerGenericTab("get_table_dependecies_for_detailed_view", this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    DbObjectDetailsViewer *detailsViewer = new DbObjectDetailsViewer("get_table_details_for_detailed_view", this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    TablePartitionsViewer *partitionInfo = new TablePartitionsViewer(this);
    addTab(partitionInfo, IconUtil::getIcon("partition"), tr("Partitions"));

    DbObjectViewerGenericTab *indexInfo = new DbObjectViewerGenericTab("get_table_indexes_for_detailed_view", this);
    desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_table_index_columns");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":object_name", objectName) <<
                 new Param(":index_owner", QString()) <<
                 new Param(":index_name", QString()) <<
                 new Param(":rs_out");
    desc->addReplacement(2, 1);
    desc->addReplacement(3, 2);

    indexInfo->setDynamicQuery(4, desc);
    addTab(indexInfo, IconUtil::getIcon("index"), tr("Indexes"));

    DbObjectDdlViewer *ddlViewer = new DbObjectDdlViewer(DbTreeModel::Table, this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));
}
