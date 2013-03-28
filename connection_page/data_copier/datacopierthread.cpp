#include "datacopierthread.h"
#include "beans/datacopieroptions.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "util/strutil.h"
#include "util/dbutil.h"
#include "defines.h"
#include <QDebug>

#define COMMIT_INTERVAL 10000

DataCopierThread::DataCopierThread(const QString &sourceSchema, DbConnection *sourceDb, const QString &targetSchema, DbConnection *targetDb, const QString &tableName, DataOperationOptions *options, const TableInfoForDataComparison &tableOptions, QObject *parent) :
    DataOperationThread(sourceSchema, sourceDb,
                        targetSchema, targetDb,
                        tableName, options,
                        tableOptions,
                        parent),
    sourceStmt(0), targetStmt(0)
{
    copyOptions = static_cast<DataCopierOptions*>(options);

    bulkHelper.setBulkSize(BULK_DATA_OPERATION_CHUNK_SIZE);
    bulkHelper.setDmlMode();
}

DataCopierThread::~DataCopierThread()
{
    delete sourceStmt;
    delete targetStmt;
}

void DataCopierThread::run()
{
    emit statusChanged(QString("Copying data for table %1").arg(this->tableName));

    QString currentTaskName="copy_table_data";

    try{
        if(tableOptions.columnsToCompare.isEmpty()){ //no columns selected for comparison
            emitCompareInfo(tableName, tr("Skipped. No columns selected."));
            emitCompletedSignal();
            return;
        }

        sourceStmt=sourceDb->createStatement();
        targetStmt=targetDb->createStatement();

        currentTaskName="prepare_bind_arrays";
        prepareBindArrays();

        currentTaskName="copy_data";
        copyData();

        emitCompareInfo(tableName, tr("Completed"));
        emitCompletedSignal();
    }catch(OciException ex){
        emit compareError(currentTaskName, ex);
    }
}

void DataCopierThread::prepareBindArrayForColumn(const QString & /*colName*/, const QString &dataType, int length, int /*colOffset*/)
{
    bulkHelper.createBufferForDataType(targetStmt, dataType, length);
}

void DataCopierThread::copyData()
{
    QString columnNamesForSelect = getColumnsForSelect(tableOptions.columnsToCompare);
    QString selectFromSourceSql=QString("SELECT %1 FROM \"%2\".\"%3\"").
            arg(columnNamesForSelect).
            arg(sourceSchema).arg(tableName);

    if(!tableOptions.whereClause.isEmpty()){
        selectFromSourceSql.append(ensureStartsWith(tableOptions.whereClause, "WHERE"));
    }

    qDebug() << "select statement from source table:" << selectFromSourceSql;

    QString columnNames = joinEnclosed(tableOptions.columnsToCompare, ",", "\"");
    QString bindVarNames;
    int colCount = tableOptions.columnsToCompare.size();
    QString bindVarName;
    for(int i=0; i<colCount; ++i){
        const QString &colName = tableOptions.columnsToCompare.at(i);
        bindVarName = QString(":B_%1").arg(i);

#ifdef ORAEXP_USE_VARCHAR_FOR_BULK_TS_AND_INTERVAL
        bindVarNames.append(DbUtil::toIntervalOrTimestamp(bindVarName, allColumns[colName]));
#else
        bindVarNames.append(bindVarName);
#endif

        if(i!=colCount-1){
            bindVarNames.append(",");
        }
    }

    QString targetTableName = tableOptions.targetTableName.isEmpty() ? this->tableName : tableOptions.targetTableName;
    QString insertIntoTargetSql = QString("INSERT INTO \"%1\".\"%2\" (%3) VALUES (%4)").
                                  arg(targetSchema, targetTableName).
                                  arg(columnNames).arg(bindVarNames);

    qDebug() << "insert into target table:" << insertIntoTargetSql;

    if(copyOptions->truncate){
        emitCompareInfo(this->tableName, tr("Truncating table"));

        QString truncateQuery = QString("TRUNCATE TABLE \"%1\".\"%2\"").arg(targetSchema, targetTableName);

        targetDb->executeQueryAndCleanup(truncateQuery);
    }

    emitCompareInfo(this->tableName, tr("Copying data"));

    QueryResult res=sourceDb->executeQuery(selectFromSourceSql);

    QScopedPointer<Statement> stmt(res.statement);
    Resultset *rs=stmt->rsAt(0);

    targetStmt->lockConnection();
    targetStmt->prepare(insertIntoTargetSql);
    targetStmt->setBindArraySize(BULK_DATA_OPERATION_CHUNK_SIZE);
    bulkHelper.bindArrays(targetStmt, "B");

    int fetchedCount = 0;
    int offset = 0;

    rs->beginFetchRows();

    while(rs->moveNext()){

        ++fetchedCount;

        for(int colIx=0; colIx<tableOptions.columnsToCompare.size(); ++colIx){
            bulkHelper.setArrayData(targetStmt, rs, colIx+1, offset);
        }

        ++offset;
        if(offset==BULK_DATA_OPERATION_CHUNK_SIZE){
            targetStmt->execute();

            emitCompareInfo(tableName, "", offset);

            emit statusChanged(QString("Copying data for %1 - %2 records completed").arg(this->tableName).arg(fetchedCount));

            offset=0;

            if(fetchedCount%COMMIT_INTERVAL==0){
                qDebug("committing");

                targetStmt->unlockConnection();
                targetDb->executeQueryAndCleanup("COMMIT");
                targetStmt->lockConnection();
            }

            if(this->stopped){
                break;
            }
        }
    }

    if(offset>0){
        if(offset < BULK_DATA_OPERATION_CHUNK_SIZE){
            bulkHelper.nullifyArrayData(targetStmt, offset);
            targetStmt->setBindArraySize(offset);
        }

        targetStmt->execute();

        emitCompareInfo(tableName, "", offset);
    }

    rs->endFetchRows();

    targetStmt->unlockConnection();

    targetDb->executeQueryAndCleanup("COMMIT");
}
