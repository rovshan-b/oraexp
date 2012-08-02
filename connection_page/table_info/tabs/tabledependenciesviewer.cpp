#include "tabledependenciesviewer.h"
#include "util/queryutil.h"

TableDependenciesViewer::TableDependenciesViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    query=QueryUtil::getQuery("get_table_dependecies_for_detailed_view");
}
