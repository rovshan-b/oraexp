#include "recordfetcherthread.h"
#include "statement.h"
#include "resultset.h"
#include "dbconnection.h"
#include "beans/statementdesc.h"
#include <memory>

#include <iostream>
using namespace std;

RecordFetcherThread::RecordFetcherThread(DbConnection *db, Resultset *rs, int recordCountToFetch,
                                         QHash<int, StatementDesc*> dynamicQueries, QObject *parent) :
    QThread(parent), db(db), rs(rs),
    recordCountToFetch(recordCountToFetch),
    dynamicQueries(dynamicQueries)
{
}

void RecordFetcherThread::run()
{
    try{
        rs->beginFetchRows();
        int columnCount=rs->getColumnCount();
        int fetchedCount=0;

        QList<QStringList> rows;
        const int chunkSize=20;
        rows.reserve(chunkSize);

        while((fetchedCount++<recordCountToFetch || recordCountToFetch==-1 /*-1 to fetch all rows*/) &&
                    rs->moveNext()){

            QStringList oneRow;
            oneRow.reserve(columnCount);

            for(int i=0; i<columnCount; i++){
                oneRow.append(rs->getAsString(i+1));
            }

            if(!dynamicQueries.isEmpty()){
                replaceValuesWithDynamicQueries(oneRow);
            }

            rows.append(oneRow);

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

void RecordFetcherThread::replaceValuesWithDynamicQueries(QList<QString> &oneRow)
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
