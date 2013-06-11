#ifndef GROUPEDDATAVIEWWIDGET_H
#define GROUPEDDATAVIEWWIDGET_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;
class QTreeView;
class QStandardItemModel;
class QSortFilterProxyModel;

class GroupedDataViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupedDataViewWidget(QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);
    void setSelectQuery(const QString &selectQuery);
    void setGroupByColumn(const QString &groupByColumn);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &fetchResult);
    void fetchCompleted(const QString &);

private:
    IQueryScheduler *queryScheduler;

    QTreeView *treeView;
    QStandardItemModel *treeModel;
    QSortFilterProxyModel *filterModel;

    QString selectQuery;
    QString groupByColumn;

    void runQuery();
};

#endif // GROUPEDDATAVIEWWIDGET_H
