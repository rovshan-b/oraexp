#ifndef SESSIONLISTTREE_H
#define SESSIONLISTTREE_H

#include <QModelIndex>

#include "widgets/ondemandinfoviewerwidget.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;
class DbUiManager;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class TreeSortFilterProxyModel;
class QVBoxLayout;
class TreeView;

class SessionListTree : public OnDemandInfoViewerWidget
{
    Q_OBJECT
public:
    explicit SessionListTree(DbUiManager *uiManager, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);
    void setSelectQuery(const QString &selectQuery);
    void setGroupByColumns(const QStringList &groupByColumns);
    QStringList getGroupByColumns() const;

    int getRecordCount() const {return this->recordCount;}

    void setFilter(const QString &filter);

    void setLocksOnly(bool locksOnly);

signals:
    void headerReady(const QStringList &columnTitles);
    void dataReady();

    void currentSessionChanged(int instId, int sid, int serial);

protected:
    virtual void doLoadInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);

    void selectionChanged(const QModelIndex & current, const QModelIndex & previous);
    void showContextMenu(const QPoint & pos);

    void actionSucceeded();
private:
    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;

    TreeView *treeView;
    QStandardItemModel *treeModel;
    TreeSortFilterProxyModel *proxyModel;

    QString selectQuery;
    QStringList groupByColumns;
    bool locksOnly;

    int lastScrollPos;
    QStringList currentGroups;
    QList<QStandardItem*> currentGroupHeaders;
    QHash<QStandardItem*, int> childCounts;

    int recordCount;

    bool autoFitColumns;
    bool selectionRestored;

    QString lastSelectedKey;

    void createTree(QVBoxLayout *mainLayout);

    void runQuery();

    QStandardItem *createGroup(const QString &groupTitle);
    QString getNewGroupIconName() const;
    void addRecord(const FetchResult &result);

    void saveSelection();
    bool restoreSelection(const QModelIndex &index);

    QString lastFilter;

    void getSelectedSessions(const QModelIndex &parent, QStringList &list, bool check = true) const;

    void displayErrorMessage(const OciException &ex);

    /*
    int instIdColumn() const;
    int sidColumn() const;
    int serialColumn() const;*/
};

#endif // SESSIONLISTTREE_H
