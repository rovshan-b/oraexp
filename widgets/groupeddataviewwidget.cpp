#include "groupeddataviewwidget.h"
#include <QtGui>

GroupedDataViewWidget::GroupedDataViewWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    treeView = new QTreeView();
    treeModel = new QStandardItemModel(this);
    filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(treeModel);
    treeView->setModel(filterModel);

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

void GroupedDataViewWidget::setGroupByColumn(const QString &groupByColumn)
{
    this->groupByColumn = groupByColumn;
}

void GroupedDataViewWidget::queryCompleted(const QueryResult &result)
{
}

void GroupedDataViewWidget::recordFetched(const FetchResult &fetchResult)
{
}

void GroupedDataViewWidget::fetchCompleted(const QString &)
{
}

void GroupedDataViewWidget::runQuery()
{
}
