#include "recordfetcherthread.h"
#include "statement.h"
#include "resultset.h"
#include "dbconnection.h"
#include "beans/statementdesc.h"
#include "defines.h"
#include <memory>

#include <iostream>
using namespace std;

RecordFetcherThread::RecordFetcherThread(DbConnection *db, Resultset *rs, int recordCountToFetch,
                                         QHash<int, StatementDesc*> dynamicQueries, QObject *parent) :
    QThread(parent),
    data(0),
    db(db), rs(rs),
    fetchStartIx(-1),
    recordCountToFetch(recordCountToFetch),
    fetchInChunks(true),
    dynamicQueries(dynamicQueries)
{
}

void RecordFetcherThread::run()
{
    try{
        rs->beginFetchRows();
        int fetchedCount=0;

        QList<QStringList> rows;
        const int chunkSize=fetchInChunks ? DB_PREFETCH_SIZE : this->recordCountToFetch;
        rows.reserve(chunkSize);

        if(this->fetchStartIx!=-1){
            if(rs->moveToPosition(this->fetchStartIx+1)){
                rows.append(getOneRow());
                ++fetchedCount;
            }
        }

        while(fetchedCount++<recordCountToFetch && rs->moveNext()){

            rows.append(getOneRow());

            if(rows.count()>=chunkSize){
                emit recordsFetched(rows);
                rows.clear();
            }
        }

        if(!rows.isEmpty()){
            emit recordsFetched(rows);
        }

        rs->endFetchRows();
    }catch(OciException &ex){
        emit fetchError(ex);
    }
    emit fetchComplete();
}

QStringList RecordFetcherThread::getOneRow() const
{
    QStringList oneRow;
    int columnCount=rs->getColumnCount();
    oneRow.reserve(columnCount);

    for(int i=0; i<columnCount; i++){
        oneRow.append(rs->getAsString(i+1));
    }

    if(!dynamicQueries.isEmpty()){
        replaceValuesWithDynamicQueries(oneRow);
    }

    return oneRow;
}

void RecordFetcherThread::setFetchRange(int startIx, int count)
{
    this->fetchStartIx=startIx;
    this->recordCountToFetch=count;
}

void RecordFetcherThread::setFetchInChunks(bool fetchInChunks)
{
    this->fetchInChunks=fetchInChunks;
}

void RecordFetcherThread::replaceValuesWithDynamicQueries(QList<QString> &oneRow) const
{
    StatementDesc* desc;

    QString values;
    QHashIterator<int, StatementDesc*> iterator(dynamicQueries);
    while (iterator.hasNext()) {
        iterator.next();
        desc=iterator.value();
        values=QString();

        rs->endFetchRows();

        try{
            Statement *stmt=desc->getStatement(db);
            desc->makeReplacements(oneRow);

            stmt->lockConnection();
            QueryResult res=stmt->execute();
            stmt->unlockConnection();
            QScopedPointer<Resultset> subRs(res.statement->rsAt(0));

            subRs->beginFetchRows();

            while(subRs->moveNext()){
                values.append(subRs->getAsString(1));
                values.append(",");
            }
            if(values.size()>0){
                values.chop(1);
            }

            oneRow.replace(iterator.key()-1, values);

            subRs->endFetchRows();
        }catch(OciException &ex){
            oneRow.replace(iterator.key()-1, ex.getErrorMessage());
        }

        rs->beginFetchRows();
    }
}
