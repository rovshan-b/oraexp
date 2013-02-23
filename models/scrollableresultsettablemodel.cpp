#include "scrollableresultsettablemodel.h"
#include "connectivity/recordfetcherthread.h"
#include "beans/fetchrangeinfo.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>
#include <QtDebug>

ScrollableResultsetTableModel::ScrollableResultsetTableModel(
        IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
        const QHash<int, StatementDesc *> &dynamicQueries,
        const QHash<QString, QString> &iconColumns,
        bool humanizeColumnNames) :
    ResultsetTableModel(queryScheduler, rs, parent, dynamicQueries,
                        iconColumns, humanizeColumnNames),
    currentOffset(-1)
{
    QAbstractItemView *parentView = qobject_cast<QAbstractItemView*>(parent);
    Q_ASSERT(parentView);
    connect(parentView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrolled(int)));
}

QVariant ScrollableResultsetTableModel::data(const QModelIndex &index, int role) const
{
    if(!isValidIndex(index, role)){
        return QVariant();
    }

    if(index.row()<currentOffset || index.row()>=currentOffset+modelData.size()){
        return QVariant();
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
    int recordCount=records.size();

    beginInsertRows(QModelIndex(), fetchedRowCount, fetchedRowCount+recordCount-1);

    if(currentOffset==-1){
        currentOffset=0;
    }

    int currentRangeEndIx = currentOffset+modelData.size()-1;
    if(modelData.size()>0 && currentRangeEndIx==fetchedRowCount-1){
        modelData.append(records);
        qDebug("Appended to model");
        shrinkModelData(true);
    }else{
        modelData=records;
        currentOffset=fetchedRowCount;
        qDebug("Replaced model");
    }

    fetchedRowCount+=records.size();

    endInsertRows();
}

void ScrollableResultsetTableModel::fetchComplete()
{
    ResultsetTableModel::fetchComplete();
    //QTimer::singleShot(0, this, SLOT(fetchData()));
    fetchData();

    //TODO: check if cursor is at the bottom and fetchData returned false, call fetchMore
}

void ScrollableResultsetTableModel::existingRecordsFetched(const QList<QStringList> &records)
{
    Q_ASSERT(records.size()>0);

    FetchRangeInfo *rangeInfo = static_cast<FetchRangeInfo*>(fetcherThread->data);

    int firstChangedRowIx, lastChangedRowIx;
    QList<QStringList> newList;
    switch(rangeInfo->fetchMode){
    case FetchRangeInfo::AppendToCurrentData:
        firstChangedRowIx = currentOffset+modelData.size()-1;
        lastChangedRowIx = currentOffset+modelData.size()+records.size()-1;

        modelData.append(records);
        qDebug("appended to model");
        break;
    case FetchRangeInfo::PrependToCurrentData:
        newList.reserve(modelData.size()+records.size());
        newList.append(records);
        newList.append(modelData);
        modelData=newList;
        currentOffset = rangeInfo->rangeStart;

        firstChangedRowIx = currentOffset;
        lastChangedRowIx = currentOffset+records.size()-1;
        qDebug("prepended to model");
        break;
    case FetchRangeInfo::ReplaceCurrentData:
        modelData=records;
        currentOffset = rangeInfo->rangeStart;

        firstChangedRowIx = currentOffset;
        lastChangedRowIx = currentOffset+modelData.size()-1;
        qDebug("replaced model");
        break;
    }


    if(rangeInfo->fetchMode==FetchRangeInfo::AppendToCurrentData ||
            rangeInfo->fetchMode==FetchRangeInfo::PrependToCurrentData){
        shrinkModelData(rangeInfo->fetchMode==FetchRangeInfo::AppendToCurrentData);
    }

    qDebug() << "record count in buffer is" << modelData.size();
    qDebug() << "current data range is from" << currentOffset << "to" << currentOffset+modelData.size()-1;

    emit dataChanged(this->index(firstChangedRowIx, columnCount()-1),
                     this->index(lastChangedRowIx, columnCount()-1));
}

void ScrollableResultsetTableModel::deleteFetcherThread()
{
    FetchRangeInfo *rangeInfo = static_cast<FetchRangeInfo*>(fetcherThread->data);
    delete rangeInfo;
    fetcherThread->data=0;

    ResultsetTableModel::deleteFetcherThread();
}

bool ScrollableResultsetTableModel::deleteResultsetOnFetchComplete() const
{
    return false;
}

void ScrollableResultsetTableModel::scrolled(int)
{
    fetchData();
}

void ScrollableResultsetTableModel::shrinkModelData(bool fromTop)
{
    int bufferSize = getCacheSize();

    int countToRemove=0;
    if(modelData.size()>bufferSize){
        countToRemove=modelData.size()-bufferSize;
    }

    if(countToRemove>0){
        qDebug() << "modelData.size()=" << modelData.size()
                 << "bufferSize=" << bufferSize
                 << ", need to remove" << countToRemove << "items";
    }else{
        qDebug() << "not shrinking. modelData.size()="
                 << modelData.size()
                 << "bufferSize=" << bufferSize;
    }

    for(int i=0; i<countToRemove; ++i){
        if(fromTop){
            modelData.removeFirst();
            ++currentOffset;
        }else{
            modelData.removeLast();
        }
    }
}

void ScrollableResultsetTableModel::fetchData()
{
    if(fetchInProgress || rs==0){
        return;
    }

    QModelIndex firstVisibleIndex, lastVisibleIndex;
    getFirstLastVisibleIndexes(firstVisibleIndex, lastVisibleIndex);

    if(!firstVisibleIndex.isValid() || !lastVisibleIndex.isValid()){
        return;
    }

    int firstRowIx=firstVisibleIndex.row();
    int lastRowIx=lastVisibleIndex.row()+1;
    //int visibleRowCount = lastRowIx - firstRowIx + 1;

    int currentFirstIx = currentOffset;
    int currentLastIx = currentOffset + modelData.size() - 1;

    //range to fetch. expand in both directions to minimize database
    //roundtrips during continuous scrolling
    int rangeStart = firstRowIx - 10;
    int rangeEnd = lastRowIx + 10;

    /*
    if(movingUp){
        //rangeStart -= visibleRowCount * 2; //fetch 2 previous pages as well
    }else{
        //rangeEnd += visibleRowCount * 2; //fetch 2 next pages as well
    }*/

    FetchRangeInfo::FetchMode fetchMode=FetchRangeInfo::ReplaceCurrentData;

    if(rangeStart >= currentFirstIx && rangeStart <= currentLastIx + 1){
        rangeStart = currentLastIx + 1;
        fetchMode=FetchRangeInfo::AppendToCurrentData;
    }


    if(rangeEnd >= currentFirstIx - 1 && rangeEnd <= currentLastIx){
        rangeEnd = currentFirstIx - 1;
        fetchMode=FetchRangeInfo::PrependToCurrentData;
    }

    rangeStart = qMax(0, rangeStart);
    rangeEnd = qMin(rowCount()-1, rangeEnd);

    if(rangeStart > rangeEnd){ //nothing to fetch
        return;
    }

    if(rangeStart>=currentFirstIx && rangeEnd<=currentLastIx){ //already in buffer
        return;
    }

    Q_ASSERT_X((rangeEnd - rangeStart + 1) < getCacheSize(),
               "buffer size calculation",
               qPrintable(QString("count to fetch (%1) must be less than max buffer size (%2)")
                          .arg(rangeEnd - rangeStart + 1)
                          .arg(getCacheSize())));

    FetchRangeInfo *rangeInfo = new FetchRangeInfo();
    rangeInfo->rangeStart=rangeStart;
    rangeInfo->rangeEnd=rangeEnd;
    rangeInfo->fetchMode=fetchMode;

    qDebug() << "current offset=" << currentOffset
             << ", current last=" << currentLastIx
             << ", first visible row=" << firstRowIx
             << ", last visible row=" << lastRowIx;
    qDebug() << "fetch range = " << rangeStart << ".." << rangeEnd << ", count=" << rangeEnd-rangeStart+1;

    queryScheduler->increaseRefCount();

    fetchInProgress=true;
    fetcherThread=new RecordFetcherThread(queryScheduler->getDb(), rs, 0, dynamicQueries, this);
    fetcherThread->data = rangeInfo;
    fetcherThread->setFetchRange(rangeStart, rangeEnd-rangeStart+1);
    fetcherThread->setFetchInChunks(false);
    connect(fetcherThread, SIGNAL(recordsFetched(QList<QStringList>)), this, SLOT(existingRecordsFetched(QList<QStringList>)));
    connect(fetcherThread, SIGNAL(fetchComplete()), this, SLOT(fetchComplete()));
    connect(fetcherThread, SIGNAL(fetchError(OciException)), this, SLOT(fetchError(OciException)));
    fetcherThread->start();
}

QTableView *ScrollableResultsetTableModel::getParentView()
{
    QObject* obj=qobject_cast<QObject*>(this);
    QTableView *parentView = qobject_cast<QTableView*>(obj->parent());
    Q_ASSERT(parentView);

    return parentView;
}

void ScrollableResultsetTableModel::getFirstLastVisibleIndexes(QModelIndex &first, QModelIndex &last)
{
    if(rowCount()==0){
        return;
    }

    QTableView *parentView = getParentView();

    first=parentView->indexAt(parentView->rect().topLeft());
    last=parentView->indexAt(parentView->rect().bottomRight());

    if(!last.isValid() && first.isValid()){
        int recordHeight = parentView->verticalHeader()->defaultSectionSize();
        int visibleRecordCount = qCeil(parentView->height() / (qreal) recordHeight);

        int lastVisibleRowIx = qMin(first.row()+visibleRecordCount+10, rowCount()-1);
        last=index(lastVisibleRowIx, columnCount()-1);
    }
}

int ScrollableResultsetTableModel::getVisibleRowCount()
{
    QModelIndex firstVisibleIndex, lastVisibleIndex;
    getFirstLastVisibleIndexes(firstVisibleIndex, lastVisibleIndex);

    if(!firstVisibleIndex.isValid() || !lastVisibleIndex.isValid()){
        return -1;
    }

    int firstRowIx=firstVisibleIndex.row();
    int lastRowIx=lastVisibleIndex.row();

    int visibleRowCount = lastRowIx - firstRowIx + 2; //must be +1. returning +2 because table sometimes reports last index as real-1
    Q_ASSERT(visibleRowCount >= 0);

    return visibleRowCount;
}

int ScrollableResultsetTableModel::getCacheSize()
{
    int cacheSize=getVisibleRowCount()*3;
    if(cacheSize<50){
        cacheSize=50;
    }

    cacheSize = qMin(cacheSize, fetchedRowCount);

    return cacheSize;
}
