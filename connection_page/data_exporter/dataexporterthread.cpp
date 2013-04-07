#include "dataexporterthread.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "connectivity/ociexception.h"
#include "connectivity/resultset.h"
#include "exporters/dataexporterbase.h"
#include "util/strutil.h"
#include <QFile>
#include <QTextCodec>

DataExporterThread::DataExporterThread(DataExporterBase *exporter,
                                       QList<QStringList> alreadyFetchedData,
                                       Resultset *rs, bool fetchToEnd,
                                       bool autoDestroy, QObject *parent) :
    StopableThread(parent),
    exporter(exporter),
    alreadyFetchedData(alreadyFetchedData),
    rs(rs),
    fetchToEnd(fetchToEnd),
    bulkMode(false),
    options(0)
{
    if(autoDestroy){
        connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    }
}

DataExporterThread::~DataExporterThread()
{
    if(!bulkMode){
        delete exporter;
    }
}

void DataExporterThread::run()
{
    QString errMessage;
    QTextStream *textStream = exporter->createOutputStream(errMessage);
    if(!textStream){
        emit exportError(errMessage);
        return;
    }

    QTextStream &out = *textStream;

    Statement *stmt = 0;

    try{
        if(bulkMode){ //working in bulk mode. must select own data
            Q_ASSERT(rs==0);
            stmt = getStatement();
            rs = stmt->rsAt(0);
            exporter->columnMetadata = rs->getColumnMetadata();
        }

        exportToStream(out);

        emit exportComplete();
    }catch(OciException &ex){
        emit exportError(ex.getErrorMessage());
    }

    delete stmt;
}

Statement *DataExporterThread::getStatement()
{
    QString query = QString("SELECT %1 FROM \"%2\".\"%3\"").arg(joinEnclosed(tableOptions.columnsToCompare),
                                                                this->schemaName,
                                                                this->tableName);
    if(!tableOptions.whereClause.isEmpty()){
        query.append(ensureStartsWith(tableOptions.whereClause, "WHERE"));
    }

    return this->queryScheduler->getDb()->executeQuery(query).statement;
}

void DataExporterThread::setOptions(IQueryScheduler *queryScheduler,
                                    const QString &schemaName,
                                    const QString &tableName,
                                    DataExporterOptions *options,
                                    const TableInfoForDataComparison &tableOptions)
{
    this->bulkMode=true;
    this->queryScheduler=queryScheduler;
    this->schemaName=schemaName;
    this->tableName=tableName;
    this->options=options;
    this->tableOptions=tableOptions;
}

void DataExporterThread::exportToStream(QTextStream &out)
{
    int exportedCount = 0;

    int startRow, startColumn, endRow, endColumn;
    if(exporter->startRow!=-1 && exporter->startColumn!=-1 &&
            exporter->endRow!=-1 && exporter->endColumn!=-1){
        startRow = exporter->startRow;
        startColumn = exporter->startColumn;
        endRow = exporter->endRow;
        endColumn = exporter->endColumn;
    }else{
        startRow = 0;
        exporter->startColumn = startColumn = 0;
        endRow = -1;
        exporter->endColumn = endColumn = exporter->columnMetadata->getColumnCount()-1;
    }

    exporter->startDocument(out);

    if(exporter->includeColumnHeaders){
        QStringList columnTitles = exporter->columnMetadata->columnTitles;
        exporter->prepareColumnHeaders(columnTitles);
        exporter->exportColumnHeaders(columnTitles, startColumn, endColumn, out);
    }

    if(rs==0 && alreadyFetchedData.size()==0){
        return;
    }

    QStringList oneRow;
    int columnCount = endColumn-startColumn+1;
    oneRow.reserve(columnCount);
    for(int i=0; i<columnCount; ++i){
        oneRow.append("");
    }

    for(int i=startRow; i<=((endRow==-1) ? alreadyFetchedData.size()-1 : endRow); ++i){
        const QStringList currentRow = alreadyFetchedData.at(i);

        for(int k=startColumn; k<=endColumn; ++k){
            oneRow[k-startColumn] = currentRow.at(k);
        }

        exporter->prepareRow(oneRow);
        exporter->exportRow(oneRow, exportedCount, out);
        ++exportedCount;

        if(exportedCount%50==0){
            emit recordsExported(exportedCount);

            if(stopped){
                break;
            }
        }
    }

    if(!stopped && fetchToEnd && endRow==-1){
        rs->beginFetchRows();
        while(rs->moveNext()){
            for(int k=startColumn; k<=endColumn; ++k){
                oneRow[k-startColumn] = rs->getAsString(k+1);
            }

            exporter->prepareRow(oneRow);
            exporter->exportRow(oneRow, exportedCount, out);
            ++exportedCount;

            if(exportedCount%50==0){
                emit recordsExported(exportedCount);

                if(stopped){
                    break;
                }
            }
        }
        rs->endFetchRows();
    }

    exporter->endDocument(out);
    out.flush();

    emit recordsExported(exportedCount);
}
