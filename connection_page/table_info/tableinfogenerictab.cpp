#include "tableinfogenerictab.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "beans/statementdesc.h"
#include "tableinfotoolbar.h"
#include <QtGui>

TableInfoGenericTab::TableInfoGenericTab(QWidget *parent) :
    TableInfoViewerWidget(parent), dt(0)
  //, retrieveResultsetAsBindParameter(false)
{
}

TableInfoGenericTab::~TableInfoGenericTab()
{
    QHashIterator<int, StatementDesc*> i(dynamicQueries);
    while (i.hasNext()) {
        i.next();

        delete i.value();
    }
}

void TableInfoGenericTab::doLoadInfo()
{
    if(isLoading()){
        return;
    }
    //refreshButton->setEnabled(false);
    toolbar->setEnabled(false);
    progressBarAction->setVisible(true);
    progressBarAction->setEnabled(true);

    TableInfoViewerWidget::doLoadInfo();

    loadData();
}

QList<Param *> TableInfoGenericTab::getQueryParams()
{
    QList<Param*> queryParams;

    queryParams.append(new Param(":owner", schemaName));
    queryParams.append(new Param(":table_name", tableName));

    return queryParams;
}

void TableInfoGenericTab::loadData()
{
    Q_ASSERT(queryScheduler);

    dt->displayQueryResults(queryScheduler, query, getQueryParams(), dynamicQueries);
}

void TableInfoGenericTab::queryCompleted()
{
    setLoadingComplete();
    progressBarAction->setVisible(false);
    //refreshButton->setEnabled(true);
    toolbar->setEnabled(true);
}

void TableInfoGenericTab::createMainWidget(QLayout *layout)
{
    dt=new DataTable();
    dt->setHumanizeColumnNames(true);
    //dt->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(queryCompleted()));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));
    layout->addWidget(dt);
}

