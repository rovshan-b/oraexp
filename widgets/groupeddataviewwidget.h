#ifndef GROUPEDDATAVIEWWIDGET_H
#define GROUPEDDATAVIEWWIDGET_H

#include "widgets/ondemandinfoviewerwidget.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class QSortFilterProxyModel;

class GroupedDataViewWidget : public OnDemandInfoViewerWidget
{
    Q_OBJECT
public:
    explicit GroupedDataViewWidget(QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);
    void setSelectQuery(const QString &selectQuery);
    void setGroupByColumns(const QStringList &groupByColumns);

protected:
    virtual void doLoadInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);

private:
    IQueryScheduler *queryScheduler;

    QTreeView *treeView;
    QStandardItemModel *treeModel;
    //QSortFilterProxyModel *filterModel;

    QString selectQuery;
    QStringList groupByColumns;

    QStringList currentGroups;
    QList<QStandardItem*> currentGroupHeaders;

    void runQuery();

    QStandardItem *createGroup(const QString &groupTitle);
    void addRecord(const QStringList &oneRow);
};

#endif // GROUPEDDATAVIEWWIDGET_H
