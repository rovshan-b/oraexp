#include "resultsettablemodel.h"
#include "connectivity/connection.h"
#include "connectivity/statement.h"
#include "connectivity/resultset.h"
#include "connectivity/recordfetcherthread.h"
#include "connectivity/resultsetdeleter.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "interfaces/iqueryscheduler.h"
#include "defines.h"
#include <QThreadPool>
#include <iostream>
#include <QMessageBox>
#include <QDebug>

using namespace std;

ResultsetTableModel::ResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                         const QHash<int, StatementDesc*> &dynamicQueries,
                                         const QHash<QString, QString> &iconColumns,
                                         bool humanizeColumnNames) :
    QAbstractTableModel(parent),
    queryScheduler(queryScheduler),
    rs(rs),
    dynamicQueries(dynamicQueries),
    humanizeColumnNames(humanizeColumnNames),
    fetchedRowCount(0),
    allDataFetched(false),
    fetcherThread(0),
    fetchInProgress(false),
    firstFetchDone(false)
    //fetchSize(DB_PREFETCH_SIZE)
{
    rsColumnCount=rs->getColumnCount()-iconColumns.size();
    columnMetadata=rs->getColumnMetadata();

    //create deep copy
    //columnIndexes.detach();

    if(iconColumns.size()>0){
        QHashIterator<QString, QString> i(iconColumns);
        while (i.hasNext()) {
            i.next();
            this->iconColumns[rs->getColumnIndexByName(i.key())]=rs->getColumnIndexByName(i.value());
        }
    }
}

ResultsetTableModel::~ResultsetTableModel()
{
    if(fetcherThread!=0){
        deleteFetcherThread();
    }

    deleteResultset();
}

void ResultsetTableModel::deleteResultset()
{
    if(rs!=0){
        QThreadPool::globalInstance()->start(new ResultsetDeleter(rs));
        rs=0;
    }
}

void ResultsetTableModel::startFetcherThread()
{
    Q_ASSERT(fetcherThread==0);

    queryScheduler->increaseRefCount();

    fetcherThread=new RecordFetcherThread(queryScheduler->getDb(), rs, 50, dynamicQueries, this);
    fetcherThread->setFetchInChunks(true);
    //if(rs->isScrollable()){
    //    fetcherThread->setFetchRange(fetchedRowCount, this->fetchSize);
    //}
    connect(fetcherThread, SIGNAL(recordsFetched(QList<QStringList>)), this, SLOT(recordsFetched(QList<QStringList>)));
    connect(fetcherThread, SIGNAL(fetchComplete()), this, SLOT(fetchComplete()));
    connect(fetcherThread, SIGNAL(fetchError(OciException)), this, SLOT(fetchError(OciException)));
    fetcherThread->start();
}

void ResultsetTableModel::deleteFetcherThread()
{
    fetcherThread->wait();
    delete fetcherThread;
    queryScheduler->decreaseRefCount();
    fetcherThread=0;
}

int ResultsetTableModel::rowCount ( const QModelIndex & ) const
{
    return fetchedRowCount;
}

int ResultsetTableModel::columnCount ( const QModelIndex & ) const
{
    return rsColumnCount;
}

bool ResultsetTableModel::canFetchMore ( const QModelIndex & ) const
{
    return !allDataFetched && !fetchInProgress;
}

void ResultsetTableModel::fetchMore ( const QModelIndex & )
{
    if(!readyToFetch()){
        qDebug("not ready to fetch");
        return;
    }

    fetchInProgress=true;

    startFetcherThread();

}

bool ResultsetTableModel::readyToFetch() const
{
    if(allDataFetched || fetchInProgress || rs==0){
        return false;
    }

    return true;
}


void ResultsetTableModel::recordsFetched(const QList<QStringList> &records)
{
    int recordCount=records.size();

    beginInsertRows(QModelIndex(), fetchedRowCount, fetchedRowCount+recordCount-1);

    modelData.append(records);
    fetchedRowCount+=records.size();

    endInsertRows();
}

void ResultsetTableModel::fetchComplete()
{
    if(rs==0){
        return;
    }

    deleteFetcherThread();

    if(!firstFetchDone){
        firstFetchDone=true;
        emit firstFetchCompleted();
    }

    if(rs->isEOF()){
        allDataFetched=true;

        if(deleteResultsetOnFetchComplete()){
            deleteResultset();
        }
    }

    fetchInProgress=false;
}

void ResultsetTableModel::fetchError(const OciException &ex)
{
    deleteFetcherThread();
    allDataFetched=true;
    deleteResultset();
    rs=0;

    QMessageBox::critical(NULL, tr("Error fetching data"), ex.getErrorMessage());
}

bool ResultsetTableModel::deleteResultsetOnFetchComplete() const
{
    return true;
}

bool ResultsetTableModel::isValidIndex(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return false;
    }

    if(role!=Qt::DisplayRole && role!=Qt::EditRole && role!=Qt::DecorationRole){
        return false;
    }

    if(role==Qt::DecorationRole && iconColumns.size()==0){
        return false;
    }

    if(index.column()>=columnMetadata->columnTitles.size() || index.column()<0){
        return false;
    }

    if(index.row()<0 || index.row()>=fetchedRowCount){
        return false;
    }

    return true;
}

QVariant ResultsetTableModel::data ( const QModelIndex & index, int role) const
{
    if(!isValidIndex(index, role)){
        return QVariant();
    }

    if(index.row()>=modelData.size()){
        return QVariant();
    }

    QList<QString> oneRow=modelData.at(index.row());

    if(role==Qt::DecorationRole){
        return getColumnIcon(oneRow, index.column()+1);
    }else{
        QString columnText = oneRow.at(index.column());
        return columnText;
    }
}

QVariant ResultsetTableModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole){
        return QVariant();
    }

    if(orientation==Qt::Vertical){
        return section+1;
    }

    if(orientation==Qt::Horizontal && (section>=columnMetadata->columnTitles.size() || section<0)){
        return QVariant();
    }

    return humanizeColumnNames ? humanize(columnMetadata->columnTitles.at(section)) : columnMetadata->columnTitles.at(section);
}

/*
void ResultsetTableModel::setFetchSize(int fetchSize)
{
    this->fetchSize=(fetchSize<DB_PREFETCH_SIZE ? DB_PREFETCH_SIZE : fetchSize);
}*/

QList<QStringList> ResultsetTableModel::getModelData() const
{
    return this->modelData;
}

Resultset *ResultsetTableModel::getResultset() const
{
    return this->rs;
}

QSharedPointer<ResultsetColumnMetadata> ResultsetTableModel::getColumnMetadata() const
{
    return this->columnMetadata;
}

bool ResultsetTableModel::isAllDataFetched() const
{
    return this->allDataFetched;
}

void ResultsetTableModel::setAllDataFetched()
{
    this->allDataFetched=true;
}

void ResultsetTableModel::setFetchInProgress(bool inProgress)
{
    this->fetchInProgress=inProgress;
}

QVariant ResultsetTableModel::getColumnIcon(const QList<QString> &oneRow, unsigned int colIx) const
{
    if(iconColumns.contains(colIx)){
        QString iconName=oneRow.at(iconColumns.value(colIx)-1);
        return IconUtil::getIcon(iconName);
    }

    return QVariant();
}

