#include "dbobjectviewergenerictab.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "beans/statementdesc.h"
#include "util/queryutil.h"
#include "util/dbutil.h"
#include <QtGui>

DbObjectViewerGenericTab::DbObjectViewerGenericTab(const QString &queryName, DbUiManager *uiManager, QWidget *parent) :
    OnDemandInfoViewerWidget(parent),
    uiManager(uiManager),
    queryScheduler(0),
    controlsCreated(false),
    setObjectTypeParam(false),
    dt(0),
    queryName(queryName),
    schemaNameCol(-1),
    objectNameCol(-1),
    objectTypeCol(-1)
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

void DbObjectViewerGenericTab::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler==0);

    this->queryScheduler=queryScheduler;
}

void DbObjectViewerGenericTab::setObjectName(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType)
{
    Q_ASSERT(this->schemaName.isEmpty());

    this->schemaName=schemaName;
    this->objectName=objectName;
    this->itemType=itemType;
}

void DbObjectViewerGenericTab::doLoadInfo()
{
    if(isLoading()){
        return;
    }
    OnDemandInfoViewerWidget::doLoadInfo();

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

void DbObjectViewerGenericTab::setObjectListMode(int schemaNameCol, int objectNameCol, int objectTypeCol, const QString &objectListSchemaName, const QString &objectListObjectType)
{
    this->schemaNameCol=schemaNameCol;
    this->objectNameCol=objectNameCol;
    this->objectTypeCol=objectTypeCol;
    this->objectListSchemaName=objectListSchemaName;
    this->objectListObjectType=objectListObjectType;
}

void DbObjectViewerGenericTab::queryCompleted()
{
    setLoadingComplete();
}

void DbObjectViewerGenericTab::createMainWidget(QLayout *layout)
{
    dt=new DataTable();
    dt->setUiManager(this->uiManager);
    dt->setHumanizeColumnNames(true);
    dt->setIconColumns(iconColumns);

    if(objectNameCol!=-1){
        dt->setObjectListMode(schemaNameCol,
                              objectNameCol,
                              objectTypeCol,
                              objectListSchemaName,
                              objectListObjectType);
    }

    //dt->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(queryCompleted()));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));
    layout->addWidget(dt);
}

void DbObjectViewerGenericTab::createChildControls()
{
    Q_ASSERT(!controlsCreated);

    this->controlsCreated=true;
    QVBoxLayout *layout=new QVBoxLayout();

    createMainWidget(layout);

    layout->setContentsMargins(2, 0, 2, 0);

    setLayout(layout);
}

QList<QAction *> DbObjectViewerGenericTab::getSpecificToolbarButtons()
{
    return QList<QAction*>();
}

