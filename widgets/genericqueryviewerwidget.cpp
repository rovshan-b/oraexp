#include "genericqueryviewerwidget.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "beans/statementdesc.h"
#include "util/queryutil.h"
#include "util/dbutil.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

GenericQueryViewerWidget::GenericQueryViewerWidget(const QString &queryName, DbUiManager *uiManager, QWidget *parent) :
    OnDemandInfoViewerWidget(parent),
    queryScheduler(0),
    queryName(queryName),
    uiManager(uiManager),
    controlsCreated(false),
    dt(0),
    schemaNameCol(-1),
    objectNameCol(-1),
    objectTypeCol(-1),
    maxColumnWidth(0)
{
}

GenericQueryViewerWidget::~GenericQueryViewerWidget()
{
    QHashIterator<int, StatementDesc*> i(dynamicQueries);
    while (i.hasNext()) {
        i.next();

        delete i.value();
    }
}

void GenericQueryViewerWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler==0);

    this->queryScheduler=queryScheduler;
}

void GenericQueryViewerWidget::doLoadInfo()
{
    if(isLoading()){
        return;
    }
    OnDemandInfoViewerWidget::doLoadInfo();

    loadData();
}

void GenericQueryViewerWidget::loadData()
{
    Q_ASSERT(queryScheduler);

    if(query.isEmpty()){
        query = QueryUtil::getQuery(queryName, queryScheduler->getDb());
    }
    dt->displayQueryResults(queryScheduler, query, getQueryParams(), dynamicQueries);
}

void GenericQueryViewerWidget::setIconColumn(const QString &displayColumnName, const QString &iconColumnName)
{
    iconColumns[displayColumnName]=iconColumnName;
}

void GenericQueryViewerWidget::setDynamicQuery(int colNum, StatementDesc *stmtDesc)
{
    Q_ASSERT(dynamicQueries.value(colNum, 0)==0);

    dynamicQueries[colNum]=stmtDesc;
}

void GenericQueryViewerWidget::queryCompleted()
{
    setLoadingComplete();
}

void GenericQueryViewerWidget::createMainWidget(QLayout *layout)
{
    dt=new DataTable();
    initDataTable();
    layout->addWidget(dt);
}

void GenericQueryViewerWidget::initDataTable()
{
    dt->setUiManager(this->uiManager);
    dt->setHumanizeColumnNames(true);
    dt->setIconColumns(iconColumns);
    if(maxColumnWidth>0){
        dt->setMaxColumnWidth(maxColumnWidth);
    }

    if(objectNameCol!=-1){
        dt->setObjectListMode(schemaNameCol,
                              objectNameCol,
                              -1,
                              objectTypeCol,
                              objectListSchemaName,
                              "",
                              objectListObjectType);
    }

    //dt->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(queryCompleted()));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));
}


void GenericQueryViewerWidget::createChildControls()
{
    Q_ASSERT(!controlsCreated);

    this->controlsCreated=true;
    QVBoxLayout *layout=new QVBoxLayout();

    createMainWidget(layout);

    layout->setContentsMargins(2, 0, 2, 0);

    setLayout(layout);
}

QList<QAction *> GenericQueryViewerWidget::getSpecificToolbarButtons()
{
    return QList<QAction*>();
}

QList<QWidget *> GenericQueryViewerWidget::getSpecificToolbarWidgets()
{
    return QList<QWidget*>();
}

void GenericQueryViewerWidget::setObjectListMode(int schemaNameCol, int objectNameCol, int objectTypeCol,
                                                 const QString &objectListSchemaName, const QString &objectListObjectType)
{
    this->schemaNameCol=schemaNameCol;
    this->objectNameCol=objectNameCol;
    this->objectTypeCol=objectTypeCol;
    this->objectListSchemaName=objectListSchemaName;
    this->objectListObjectType=objectListObjectType;
}

void GenericQueryViewerWidget::setMaxColumnWidth(int maxColumnWidth)
{
    this->maxColumnWidth = maxColumnWidth;
}
