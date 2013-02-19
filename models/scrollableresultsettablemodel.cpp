#include "scrollableresultsettablemodel.h"
#include "connectivity/recordfetcherthread.h"
#include <QtGui>

#define MAX_CACHE_SIZE 500

ScrollableResultsetTableModel::ScrollableResultsetTableModel(
        IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
        const QHash<int, StatementDesc *> &dynamicQueries,
        const QHash<QString, QString> &iconColumns,
        bool humanizeColumnNames) :
    ResultsetTableModel(queryScheduler, rs, parent, dynamicQueries,
                        iconColumns, humanizeColumnNames),
    currentOffset(0), currentFetchMode(FetchMore)
{
}

QVariant ScrollableResultsetTableModel::data(const QModelIndex &index, int role) const
{
    if(!isValidIndex(index, role)){
        return QVariant();
    }

    if(index.row()<currentOffset || index.row()>=currentOffset+modelData.size()){
        return "fetching...";
    }

    int realRowIx = index.row()-currentOffset;
    Q_ASSERT(realRowIx>=0 && realRowIx<modelData.size());

    QList<QString> oneRow=modelData.at(realRowIx);

    if(role==Qt::DecorationRole){
        return getColumnIcon(oneRow, index.column()+1);
    }else{
        QString columnText = oneRow.at(index.column());
        return columnText;
    }
}

void ScrollableResultsetTableModel::recordsFetched(const QList<QStringList> &records)
{
    if(currentFetchMode==FetchMore){
        int recordCount=records.size();

        beginInsertRows(QModelIndex(), fetchedRowCount, fetchedRowCount+recordCount-1);

        this->modelData=records;
        this->currentOffset=fetchedRowCount-1;
        fetchedRowCount+=records.size();

        endInsertRows();
    }else{
        this->modelData=records;
        this->currentOffset=this->currentOffsetInProgress;

        emit dataChanged(this->index(currentOffset, columnCount()-1),
                         this->index(currentOffset+modelData.size()-1, columnCount()));
    }
}

void ScrollableResultsetTableModel::createFetcherThread()
{
    ResultsetTableModel::createFetcherThread();
    if(currentFetchMode==FetchCurrent){
        //fetcherThread->setFetchRange();
    }
}

bool ScrollableResultsetTableModel::deleteResultsetOnFetchComplete() const
{
    return false;
}

