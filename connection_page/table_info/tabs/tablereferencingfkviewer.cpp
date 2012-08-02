#include "tablereferencingfkviewer.h"
#include "util/queryutil.h"
#include "beans/statementdesc.h"

TableReferencingFKViewer::TableReferencingFKViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    query=QueryUtil::getQuery("get_table_referencing_fks_for_detailed_view");
}

void TableReferencingFKViewer::setTableName(const QString &schemaName, const QString &tableName)
{
    TableInfoGenericTab::setTableName(schemaName, tableName);

    StatementDesc *desc=new StatementDesc();
    desc->query=QueryUtil::getQuery("get_constraint_column_names");
    desc->params=QList<Param*>() <<
                 new Param(":owner", schemaName) <<
                 new Param(":constraint_name", QString());
    desc->addReplacement(1, 3);

    dynamicQueries[4]=desc;
}
