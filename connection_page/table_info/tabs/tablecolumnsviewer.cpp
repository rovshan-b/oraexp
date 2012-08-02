#include "tablecolumnsviewer.h"
#include "util/queryutil.h"
#include "../tableinfotoolbar.h"
#include "util/iconutil.h"
#include <QtGui>

TableColumnsViewer::TableColumnsViewer(QWidget *parent) :
    TableInfoGenericTab(parent)
{

}

void TableColumnsViewer::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    TableInfoGenericTab::setQueryScheduler(queryScheduler);

    query=QueryUtil::getQuery("get_table_columns_for_detailed_view", queryScheduler->getDb());
}

void TableColumnsViewer::addSpecificToolbarButtons()
{
    toolbar->addSeparator();
    QAction *detailedViewAction=toolbar->addAction(IconUtil::getIcon("advanced_options"), tr("Show all details"));
    detailedViewAction->setCheckable(true);
    connect(detailedViewAction, SIGNAL(toggled(bool)), this, SLOT(detailedViewToggled(bool)));
}

void TableColumnsViewer::detailedViewToggled(bool showDetailedView)
{
    QString queryName = showDetailedView ? "get_columns_details_for_detailed_view" : "get_table_columns_for_detailed_view";
    query = QueryUtil::getQuery(queryName, queryScheduler->getDb());

    refreshInfo();
}
