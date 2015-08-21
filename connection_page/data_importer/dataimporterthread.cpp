#include "dataimporterthread.h"
#include "columnmapping.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "importers/csvimporter.h"
#include "util/strutil.h"
#include "defines.h"
#include <QDebug>

#define BIND_VAR_PREFIX "B"

DataImporterThread::DataImporterThread(const QString &sourceSchema,
                                       DbConnection *sourceDb,
                                       const QString &tableName,
                                       const QList<ColumnMapping *> &columnMappings,
                                       const QString &beforeImportQuery,
                                       const QString &insertQuery,
                                       const QString &afterImportQuery, CsvImporter *importer, QObject *parent) :
    DataOperationThread(sourceSchema, sourceDb, "", 0, tableName,0, TableInfoForDataComparison(), parent),
    columnMappings(columnMappings),
    beforeImportQuery(beforeImportQuery),
    insertQuery(insertQuery),
    afterImportQuery(afterImportQuery),
    importer(importer)
{
    bulkHelper.setBulkSize(BULK_DATA_OPERATION_CHUNK_SIZE);
    bulkHelper.setDmlMode();
}

DataImporterThread::~DataImporterThread()
{
    qDeleteAll(columnMappings);
    delete stmt;
}

void DataImporterThread::run()
{
    try{
        stmt = sourceDb->createStatement();

        importData();

        emitCompletedSignal();
    }catch(OciException ex){
        if(stmt->hasLockOnConnection()){
            stmt->unlockConnection();
        }

        rollback();

        emit compareError("import_data", ex);
    }
}

void DataImporterThread::importData()
{
    if(!beforeImportQuery.isEmpty()){
        emit statusChanged(tr("Executing before import query..."));
        sourceDb->executeQueryAndCleanup(beforeImportQuery);
    }

    emit statusChanged(tr("Importing data"));

    QStringList fileFieldNames;
    for(int i=0; i<columnMappings.size(); ++i){
        ColumnMapping *mapping = columnMappings.at(i);
        tableOptions.addColumnToCompare(mapping->columnName);

        fileFieldNames.append(mapping->fileFieldName);
    }

    QStringList originalFileFieldNames = fileFieldNames;

    qSort(fileFieldNames.begin(), fileFieldNames.end(), stringLengthDescLessThan); //first sort by length in descending order to prevent in name replacements
    for(int i=0; i<fileFieldNames.size(); ++i){
        insertQuery.replace(QString(":%1").arg(fileFieldNames.at(i)),
                            QString(":%1_%2").arg(BIND_VAR_PREFIX).arg(originalFileFieldNames.indexOf(fileFieldNames.at(i))), Qt::CaseInsensitive);
    }


    prepareBindArrays(true);

    stmt->lockConnection();
    stmt->prepare(insertQuery);
    stmt->setBindArraySize(BULK_DATA_OPERATION_CHUNK_SIZE);
    bulkHelper.bindArrays(stmt, BIND_VAR_PREFIX);
    currentOffset = 0;

    importer->resetPosition();
    importer->resetBuffer();
    importer->readRows(this);

    if(currentOffset>0){
        if(currentOffset < BULK_DATA_OPERATION_CHUNK_SIZE){
            bulkHelper.nullifyArrayData(stmt, currentOffset);
            stmt->setBindArraySize(currentOffset);
        }

        stmt->execute();
        emit chunkImported(currentOffset);
    }

    stmt->unlockConnection();

    if(!afterImportQuery.isEmpty() && !stopped){
        emit statusChanged(tr("Executing after import query..."));
        sourceDb->executeQueryAndCleanup(afterImportQuery);
    }

    if(stopped){
        rollback();
    }
}

void DataImporterThread::rollback()
{
    emit statusChanged(tr("Rolling back transaction..."));
    try{sourceDb->executeQueryAndCleanup("ROLLBACK");}catch(OciException&){}
}

void DataImporterThread::prepareBindArrayForColumn(const QString & /*colName*/, const QString &dataType, int length, int /*colOffset*/)
{
    bulkHelper.createBufferForDataType(stmt, dataType, length);
}

void DataImporterThread::headerAvailable(const QStringList &headerTitles)
{
    Q_UNUSED(headerTitles);
}

void DataImporterThread::rowAvailable(const QStringList &values)
{
    int valueCount = values.size();
    int colCount = tableOptions.columnsToCompare.size();

    for(int colIx=0; colIx<colCount; ++colIx){
        bulkHelper.setArrayData(stmt, (colIx>=valueCount || colIx>=columnMappings.count()) ?
                                    ""
                                  :
                                    values.at(columnMappings.at(colIx)->fileFieldIx) ,
                                colIx+1, currentOffset);
    }

    ++currentOffset;

    if(currentOffset==BULK_DATA_OPERATION_CHUNK_SIZE){
        stmt->execute();
        stmt->setCurrentOffset(stmt->getCurrentOffset() + BULK_DATA_OPERATION_CHUNK_SIZE);

        emit chunkImported(currentOffset);

        currentOffset=0;

        if(this->stopped){
            importer->stop();
        }
    }
}
