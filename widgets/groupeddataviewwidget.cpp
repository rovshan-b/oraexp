#include "groupeddataviewwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "util/strutil.h"
#include <QDebug>
#include <QtGui>

GroupedDataViewWidget::GroupedDataViewWidget(QWidget *parent) :
    OnDemandInfoViewerWidget(parent), queryScheduler(0)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    treeView = new QTreeView();
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setUniformRowHeights(true);
    treeView->setRootIsDecorated(true);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    treeModel = new QStandardItemModel(this);
    //filterModel = new QSortFilterProxyModel(this);
    //filterModel->setSourceModel(treeModel);
    treeView->setModel(treeModel);

    mainLayout->addWidget(treeView);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void GroupedDataViewWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    Q_ASSERT(!selectQuery.isEmpty());
}

void GroupedDataViewWidget::setSelectQuery(const QString &selectQuery)
{
    this->selectQuery = selectQuery;
}

void GroupedDataViewWidget::setGroupByColumns(const QStringList &groupByColumns)
{
    this->groupByColumns = groupByColumns;
    int count = groupByColumns.size();

    for(int i=count-1; i>=0; --i){
        int ix = this->groupByColumns.indexOf(this->groupByColumns.at(i)); //check if column is repeated
        if(ix != i){
            this->groupByColumns.removeAt(i); //if repeated remove later occurence
        }
    }
}

void GroupedDataViewWidget::doLoadInfo()
{
    Q_ASSERT(queryScheduler);

    if(isLoading()){
        return;
    }

    OnDemandInfoViewerWidget::doLoadInfo();

    runQuery();
}

void GroupedDataViewWidget::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        setLoadingComplete();
        return;
    }
}

void GroupedDataViewWidget::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        return;
    }

    if(treeView->header()->count() == 0){
        treeModel->setHorizontalHeaderLabels(humanizeList(result.columnTitles));
    }

    Q_ASSERT(groupByColumns.size() >= currentGroups.size());

    if(groupByColumns.size() > 0){
        for(int i=0; i<currentGroups.size(); ++i){
            QString groupColumn = groupByColumns.at(i);
            QString groupValue = result.colValue(groupColumn);

            if(groupValue != currentGroups.at(i)){ //remove all groups starting with current
                int from = currentGroups.size() -1;
                int to = i;
                for(int k=from; k>=to; --k){
                    currentGroups.removeAt(k);
                    currentGroupHeaders.removeAt(k);
                }
                break;
            }
        }
    }

    if(currentGroups.size() < groupByColumns.size()){
        int from = currentGroups.size();
        int to = groupByColumns.size();
        for(int i=from; i<to; ++i){
            QString newGroupColumn = groupByColumns.at(i);
            QString newGroupValue = result.colValue(newGroupColumn);

            createGroup(newGroupValue);
        }
    }

    addRecord(result.oneRow);
}

QStandardItem *GroupedDataViewWidget::createGroup(const QString &groupTitle)
{
    QStandardItem *lastGroupHeader = currentGroupHeaders.size()==0 ? treeModel->invisibleRootItem() : currentGroupHeaders.last();
    QStandardItem *newGroupHeader = new QStandardItem(groupTitle);

    QList<QStandardItem*> row;
    row.append(newGroupHeader);
    //add placeholders to enable full row selection
    for(int i=0; i<treeModel->columnCount()-1; ++i){
        row.append(new QStandardItem());
    }

    lastGroupHeader->appendRow(row);

    currentGroups.append(groupTitle);
    currentGroupHeaders.append(newGroupHeader);

    return newGroupHeader;
}

void GroupedDataViewWidget::addRecord(const QStringList &oneRow)
{
    QStandardItem *lastGroupHeader = currentGroupHeaders.size()==0 ? treeModel->invisibleRootItem() : currentGroupHeaders.last();

    QList<QStandardItem*> items;
    for(int i=0; i<oneRow.size(); ++i){
        items.append(new QStandardItem((oneRow.at(i))));
    }

    lastGroupHeader->appendRow(items);
}

void GroupedDataViewWidget::fetchCompleted(const QString &)
{
    setLoadingComplete();

    treeView->setUpdatesEnabled(false);
    treeView->expandAll();
    for(int i=0; i<treeView->header()->count(); ++i){
        treeView->resizeColumnToContents(i);
    }
    treeView->setUpdatesEnabled(true);
}

void GroupedDataViewWidget::runQuery()
{
    QString queryToExecute = this->selectQuery;

    if(groupByColumns.size() > 0){
        queryToExecute.append(" ORDER BY ").append(groupByColumns.join(", "));
    }

    queryScheduler->enqueueQuery(QString("$%1").arg(queryToExecute),
                                 QList<Param*>(),
                                 this,
                                 "get_session_list",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}
