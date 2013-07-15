#include "sessionlisttree.h"
#include "interfaces/iqueryscheduler.h"
#include "util/strutil.h"
#include "util/iconutil.h"
#include "widgets/treeview.h"
#include "models/treesortfilterproxymodel.h"
#include "connectivity/dbconnection.h"
#include <QDebug>
#include <QtGui>

#define RESERVED_COL_COUNT 1

SessionListTree::SessionListTree(QWidget *parent) :
    OnDemandInfoViewerWidget(parent), queryScheduler(0), recordCount(0), autoFitColumns(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    createTree(mainLayout);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void SessionListTree::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    Q_ASSERT(!selectQuery.isEmpty());
}

void SessionListTree::createTree(QVBoxLayout *mainLayout)
{
    treeView = new TreeView();
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setUniformRowHeights(true);
    treeView->setRootIsDecorated(true);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setSelectionMode(QAbstractItemView::ContiguousSelection);
    treeView->setSortingEnabled(true);

    treeModel = new QStandardItemModel(this);
    treeModel->sort(-1);
    proxyModel = new TreeSortFilterProxyModel(this);
    proxyModel->setSourceModel(treeModel);
    treeView->setModel(proxyModel);

    mainLayout->addWidget(treeView);

    connect(treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

void SessionListTree::setSelectQuery(const QString &selectQuery)
{
    this->selectQuery = selectQuery;
}

void SessionListTree::setGroupByColumns(const QStringList &groupByColumns)
{
    this->groupByColumns = groupByColumns;
    int count = groupByColumns.size();

    for(int i=count-1; i>=0; --i){
        int ix = this->groupByColumns.indexOf(this->groupByColumns.at(i)); //check if column is repeated
        if(ix != i){
            this->groupByColumns.removeAt(i); //if repeated remove later occurence
        }
    }

    treeView->setRootIsDecorated(this->groupByColumns.size() > 0);
    autoFitColumns = true;
}

QStringList SessionListTree::getGroupByColumns() const
{
    return this->groupByColumns;
}

void SessionListTree::setFilter(const QString &filter)
{
    lastFilter = QRegExp(filter);
    lastFilter.setCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterRegExp(lastFilter);
}

void SessionListTree::doLoadInfo()
{
    Q_ASSERT(queryScheduler);

    if(isLoading()){
        return;
    }

    OnDemandInfoViewerWidget::doLoadInfo();

    runQuery();
}

void SessionListTree::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        setLoadingComplete();
        return;
    }

    //cleanup
    lastScrollPos = treeView->verticalScrollBar()->value();
    saveSelection();
    treeView->setUpdatesEnabled(false);
    treeModel->removeRows(0, treeModel->rowCount());
    currentGroups.clear();
    currentGroupHeaders.clear();
    childCounts.clear();
    recordCount = 0;
    proxyModel->setFilterRegExp("");

    proxyModel->setDynamicSortFilter(false);
}

void SessionListTree::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        proxyModel->setFilterRegExp(lastFilter);
        treeView->setUpdatesEnabled(true);
        return;
    }

    if(treeView->header()->count() == 0){
        QStringList colTitles = result.columnTitles;
        for(int i=0; i<RESERVED_COL_COUNT; ++i){
            colTitles.removeLast();
        }
        treeModel->setHorizontalHeaderLabels(humanizeList(colTitles));
        emit headerReady(colTitles);
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

    addRecord(result);
}

QStandardItem *SessionListTree::createGroup(const QString &groupTitle)
{
    QStandardItem *lastGroupHeader = currentGroupHeaders.size()==0 ? treeModel->invisibleRootItem() : currentGroupHeaders.last();
    QStandardItem *newGroupHeader = new QStandardItem(IconUtil::getIcon(getNewGroupIconName()), groupTitle);
    newGroupHeader->setData(1);

    QList<QStandardItem*> row;
    row.append(newGroupHeader);
    //add placeholders to enable full row selection
    for(int i=0; i<treeModel->columnCount()-1-RESERVED_COL_COUNT; ++i){
        row.append(new QStandardItem());
    }

    lastGroupHeader->appendRow(row);

    QModelIndex newIndex = proxyModel->mapFromSource(newGroupHeader->index());

    treeView->expand(newIndex);
    if(newIndex.isValid() && !selectionRestored){
        selectionRestored = restoreSelection(newIndex);
    }

    currentGroups.append(groupTitle);
    currentGroupHeaders.append(newGroupHeader);

    return newGroupHeader;
}

QString SessionListTree::getNewGroupIconName() const
{
    QString currentGroupBy = groupByColumns.at(currentGroups.size());

    QString result;

    if(currentGroupBy == "USERNAME" ||
            currentGroupBy == "OSUSER"){
        result = "user";
    }else if(currentGroupBy == "MACHINE"){
        result = "computer";
    }else if(currentGroupBy == "MODULE"){
        result = "green_flag";
    }else{
        result = "list_group";
    }

    return result;
}

void SessionListTree::addRecord(const FetchResult &result)
{
    QStandardItem *lastGroupHeader = currentGroupHeaders.size()==0 ? treeModel->invisibleRootItem() : currentGroupHeaders.last();

    bool isCurrent = result.colValue("IS_CURRENT") == "1";

    QList<QStandardItem*> items;
    const QStringList &oneRow = result.oneRow;
    for(int i=0; i<oneRow.size()-RESERVED_COL_COUNT; ++i){
        QStandardItem *newItem = new QStandardItem((oneRow.at(i)));
        items.append(newItem);

        if(i==0){
            newItem->setData(2);
        }

        if(isCurrent){
            newItem->setBackground(Qt::green);
        }
    }

    lastGroupHeader->appendRow(items);

    QModelIndex proxyModelIndex = proxyModel->mapFromSource(items.at(0)->index());
    if(proxyModelIndex.isValid() && !selectionRestored){
        selectionRestored = restoreSelection(proxyModelIndex);
    }

    ++recordCount;

    foreach(QStandardItem *groupHeader, currentGroupHeaders){
        ++childCounts[groupHeader];
    }
}

void SessionListTree::saveSelection()
{
    lastSelectedKey = "";
    selectionRestored = false;
    QModelIndex currentIndex = treeView->currentIndex();

    if(!currentIndex.isValid()){
        return;
    }

    const QAbstractItemModel *model = currentIndex.model();
    int row = currentIndex.row();
    QModelIndex parent = currentIndex.parent();
    if(currentIndex.isValid()){
        QString instId = model->index(row, 0, parent).data().toString();
        QString sid = model->index(row, 1, parent).data().toString();
        QString serial = model->index(row, 2, parent).data().toString();

        if(instId.endsWith(")")){ //group header with child count in brackets
            instId.remove(instId.lastIndexOf(" ("), 100);
        }

        lastSelectedKey = QString("%1,%2,%3").arg(instId, sid, serial);
    }
}

bool SessionListTree::restoreSelection(const QModelIndex &index)
{
    if(lastSelectedKey.isEmpty()){
        return true;
    }

    Q_ASSERT(index.column() == 0);

    QStringList parts = lastSelectedKey.split(",");
    Q_ASSERT(parts.size()==3);

    const QAbstractItemModel *model = index.model();
    int row = index.row();
    QModelIndex parent = index.parent();
    QString instId = model->index(row, 0, parent).data().toString();
    QString sid = model->index(row, 1, parent).data().toString();
    QString serial = model->index(row, 2, parent).data().toString();

    if(instId == parts.at(0) &&
            sid == parts.at(1) &&
            serial == parts.at(2)){
        treeView->selectionModel()->select(index,
                                           QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Current|QItemSelectionModel::Rows);
        treeView->setCurrentIndex(index);
        return true;
    }

    return false;
}

void SessionListTree::fetchCompleted(const QString &)
{
    setLoadingComplete();

    //treeView->setUpdatesEnabled(false);
    if(autoFitColumns){
        treeView->resizeColumnsToContents();
    }
    if(treeView->verticalScrollBar()->maximum() < lastScrollPos){
        treeView->verticalScrollBar()->setRange(0, lastScrollPos);
    }
    proxyModel->setFilterRegExp(lastFilter);
    treeView->verticalScrollBar()->setValue(lastScrollPos);
    treeView->setUpdatesEnabled(true);

    QHashIterator<QStandardItem*, int> i(childCounts);
    while (i.hasNext()) {
        i.next();

        i.key()->setText(QString("%1 (%2)").arg(i.key()->text()).arg(i.value()));
    }

    proxyModel->setDynamicSortFilter(true);

    if(autoFitColumns){
        autoFitColumns = false;
    }

    emit dataReady();
}

void SessionListTree::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(!queryScheduler){
        return;
    }

    Q_UNUSED(previous);

    if(!current.isValid()){
        return;
    }

    const QAbstractItemModel *model = current.model();

    QString instId, sid, serial;

    if(queryScheduler->getDb()->supportsGlobalPerformanceViews()){
        instId = model->index(current.row(), 0, current.parent()).data().toString();
        sid = model->index(current.row(), 1, current.parent()).data().toString();
        serial = model->index(current.row(), 2, current.parent()).data().toString();
    }else{
        instId = "-1";
        sid = model->index(current.row(), 0, current.parent()).data().toString();
        serial = model->index(current.row(), 1, current.parent()).data().toString();
    }

    if(sid.isEmpty() || serial.isEmpty()){ //selection is a group
        return;
    }

    emit currentSessionChanged(instId.toInt(),
                               sid.toInt(),
                               serial.toInt());
}

void SessionListTree::runQuery()
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