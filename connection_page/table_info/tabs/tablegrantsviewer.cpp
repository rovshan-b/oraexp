#include "tablegrantsviewer.h"
#include "util/queryutil.h"

TableGrantsViewer::TableGrantsViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    query=QueryUtil::getQuery("get_table_grants_for_detailed_view");
}
