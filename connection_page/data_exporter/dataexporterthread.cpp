#include "dataexporterthread.h"
#include "connectivity/ociexception.h"
#include "connectivity/resultset.h"
#include "exporters/dataexporterbase.h"
#include <QFile>
#include <QTextCodec>

DataExporterThread::DataExporterThread(DataExporterBase *exporter,
                                       QList<QStringList> alreadyFetchedData,
                                       Resultset *rs, bool fetchToEnd, QObject *parent) :
    QThread(parent),
    exporter(exporter),
    alreadyFetchedData(alreadyFetchedData),
    rs(rs),
    fetchToEnd(fetchToEnd),
    stopped(false)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

DataExporterThread::~DataExporterThread()
{
    delete exporter;
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

    try{
        exportToStream(out);

        emit exportComplete();
    }catch(OciException &ex){
        emit exportError(ex.getErrorMessage());
    }
}

void DataExporterThread::stop()
{
    this->stopped=true;
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
