#include "dbobjectviewergenerictab.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "beans/statementdesc.h"
#include "util/queryutil.h"
#include "util/dbutil.h"
#include <QtGui>

DbObjectViewerGenericTab::DbObjectViewerGenericTab(const QString &queryName, QWidget *parent) :
    DbObjectViewerWidget(parent), setObjectTypeParam(false), dt(0), queryName(queryName)
{
}

DbObjectViewerGenericTab::~DbObjectViewerGenericTab()
{
    QHashIterator<int, StatementDesc*> i(dynamicQueries);
    while (i.hasNext()) {
        i.next();

        delete i.value();
    }
}

void DbObjectViewerGenericTab::doLoadInfo()
{
    if(isLoading()){
        return;
    }
    DbObjectViewerWidget::doLoadInfo();

    loadData();
}

QList<Param *> DbObjectViewerGenericTab::getQueryParams()
{
    QList<Param*> queryParams;

    queryParams.append(new Param(":owner", schemaName));
    queryParams.append(new Param(":object_name", objectName));

    if(setObjectTypeParam){
        queryParams.append(new Param(":object_type", DbUtil::getDbObjectTypeNameByNodeType(itemType)));
    }

    return queryParams;
}

void DbObjectViewerGenericTab::loadData()
{
    Q_ASSERT(queryScheduler);

    if(query.isEmpty()){
        query = QueryUtil::getQuery(queryName, queryScheduler->getDb());
    }
    dt->displayQueryResults(queryScheduler, query, getQueryParams(), dynamicQueries);
}

void DbObjectViewerGenericTab::setIconColumn(const QString &displayColumnName, const QString &iconColumnName)
{
    iconColumns[displayColumnName]=iconColumnName;
}

void DbObjectViewerGenericTab::setDynamicQuery(int colNum, StatementDesc *stmtDesc)
{
    Q_ASSERT(dynamicQueries.value(colNum, 0)==0);

    dynamicQueries[colNum]=stmtDesc;
}

void DbObjectViewerGenericTab::queryCompleted()
{
    setLoadingComplete();
}

void DbObjectViewerGenericTab::createMainWidget(QLayout *layout)
{
    dt=new DataTable();
    dt->setHumanizeColumnNames(true);
    dt->setIconColumns(iconColumns);
    //dt->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(queryCompleted()));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));
    layout->addWidget(dt);
}

