#include "dbtreeview.h"
#include "navtree/dbtreeitem.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

DbTreeView::DbTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setHeaderHidden(true);
    setUniformRowHeights(true);
    setRootIsDecorated(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
}

DbTreeModel *DbTreeView::getModel() const
{
    QAbstractItemModel *currentModel=model();
    if(currentModel==0){
        return 0;
    }

    return static_cast<DbTreeModel*>(currentModel);
}

void DbTreeView::setQueryScheduler(IQueryScheduler *queryScheduler,
                               const QString &defaultSchemaName,
                               DbUiManager *uiManager, bool enableHorizontalScrollbar)
{
    this->defaultSchemaName=defaultSchemaName;

    QAbstractItemModel *currentModel=model();
    QItemSelectionModel *currentSelectionModel=selectionModel();

    if(currentModel!=0){
        delete currentModel;
    }

    if(currentSelectionModel!=0){
        delete currentSelectionModel;
    }

    DbTreeModel *treeModel=new DbTreeModel(queryScheduler, defaultSchemaName, this);
    treeModel->setUiManager(uiManager);
    connect(treeModel, SIGNAL(childrenPopulated(QModelIndex)), this, SIGNAL(childrenPopulated(QModelIndex)));
    connect(treeModel, SIGNAL(childrenPopulateError(QModelIndex,OciException)), this, SIGNAL(childrenPopulateError(QModelIndex,OciException)));
    connect(treeModel, SIGNAL(childrenPopulateError(const QModelIndex &, const OciException &)), this, SLOT(handleChildrenPopulateError(const QModelIndex &, const OciException &)));
    setModel(treeModel);

    if(enableHorizontalScrollbar){
        header()->setResizeMode(QHeaderView::ResizeToContents);
        header()->setStretchLastSection(false);
    }

    emit treeViewReady();
}

IQueryScheduler *DbTreeView::getQueryScheduler() const
{
    return getModel()->getQueryScheduler();
}

void DbTreeView::showContextMenu(const QPoint &pos)
{
    QModelIndex ix=indexAt(pos);
    if(!ix.isValid()){
        return;
    }

    DbTreeItem* node=(DbTreeItem*)ix.internalPointer();
    QMenu *nodeMenu=node->getContextMenu(ix);

    if(nodeMenu==0){
        return;
    }

    try
    {
        nodeMenu->exec(mapToGlobal(pos));
    }
    catch(std::exception &ex)
    {
        QMessageBox::critical(this, "Error while executing action", ex.what());
    }

    qDeleteAll(nodeMenu->actions());
    delete nodeMenu;
}

void DbTreeView::checkAll(const QModelIndex &parent, bool check)
{
    DbTreeModel *currentModel=static_cast<DbTreeModel*>(model());
    if(currentModel!=0){
        currentModel->checkAll(parent, check);
    }
}

void DbTreeView::handleChildrenPopulateError(const QModelIndex &/*parent*/, const OciException &exception)
{
    QMessageBox::critical(this->window(), tr("Error retrieving child nodes"), exception.getErrorMessage());
}
