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
class QVBoxLayout;
class TreeView;

class GroupedDataViewWidget : public OnDemandInfoViewerWidget
{
    Q_OBJECT
public:
    explicit GroupedDataViewWidget(QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);
    void setSelectQuery(const QString &selectQuery);
    void setGroupByColumns(const QStringList &groupByColumns);
    QStringList getGroupByColumns() const;

    int getRecordCount() const {return this->recordCount;}

signals:
    void headerReady(const QStringList &columnTitles);
    void dataReady();

protected:
    virtual void doLoadInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);

private:
    IQueryScheduler *queryScheduler;

    TreeView *treeView;
    QStandardItemModel *treeModel;

    QString selectQuery;
    QStringList groupByColumns;

    int lastScrollPos;
    QStringList currentGroups;
    QList<QStandardItem*> currentGroupHeaders;
    QHash<QStandardItem*, int> childCounts;

    int recordCount;

    void createTree(QVBoxLayout *mainLayout);

    void runQuery();

    QStandardItem *createGroup(const QString &groupTitle);
    QString getNewGroupIconName() const;
    void addRecord(const QStringList &oneRow);
};

#endif // GROUPEDDATAVIEWWIDGET_H
