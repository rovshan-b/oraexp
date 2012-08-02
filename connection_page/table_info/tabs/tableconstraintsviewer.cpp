#include "tableconstraintsviewer.h"
#include "util/queryutil.h"
#include "widgets/datatable.h"

TableConstraintsViewer::TableConstraintsViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{
    query=QueryUtil::getQuery("get_table_constraints_for_detailed_view");
}

void TableConstraintsViewer::createMainWidget(QLayout *layout)
{
    TableInfoGenericTab::createMainWidget(layout);

    dt->setIconColumn("CONSTRAINT_NAME", "TYPE_ICON");
}
