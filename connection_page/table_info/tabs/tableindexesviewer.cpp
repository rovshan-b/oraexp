#include "tableindexesviewer.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "beans/statementdesc.h"
#include "util/queryutil.h"

TableIndexesViewer::TableIndexesViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    query=QueryUtil::getQuery("get_table_indexes_for_detailed_view");
}

void TableIndexesViewer::setTableName(const QString &schemaName, const QString &tableName)
{
    TableInfoGenericTab::setTableName(schemaName, tableName);

    StatementDesc *desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_table_index_columns");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":table_name", tableName) <<
                 new Param(":index_owner", QString()) <<
                 new Param(":index_name", QString()) <<
                 new Param(":rs_out");
    desc->addReplacement(2, 1);
    desc->addReplacement(3, 2);

    dynamicQueries[4]=desc;
}
