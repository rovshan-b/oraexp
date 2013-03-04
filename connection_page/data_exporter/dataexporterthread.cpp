#include "dataexporterthread.h"
#include "connectivity/ociexception.h"
#include "connectivity/resultset.h"
#include "exporters/dataexporterbase.h"
#include <QFile>

DataExporterThread::DataExporterThread(DataExporterBase *exporter,
                                       const QStringList &columnTitles,
                                       QList<QStringList> alreadyFetchedData,
                                       Resultset *rs, bool fetchToEnd, QObject *parent) :
    QThread(parent),
    exporter(exporter),
    columnTitles(columnTitles),
    alreadyFetchedData(alreadyFetchedData),
    rs(rs),
    fetchToEnd(fetchToEnd)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

DataExporterThread::~DataExporterThread()
{
    delete exporter;
}

void DataExporterThread::run()
{
    QFile file(exporter->filename);
    if(!file.open(QIODevice::WriteOnly)){
        emit exportError(file.errorString());
        return;
    }

    QTextStream out(&file);

    try{
        exportToStream(out);

        emit exportComplete();
    }catch(OciException &ex){
        emit exportError(ex.getErrorMessage());
    }
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
        startColumn = 0;
        endRow = -1;
        endColumn = columnTitles.size()-1;
    }

    exporter->exportColumnHeaders(columnTitles, startColumn, endColumn, out);

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

        ++exportedCount;
        exporter->exportRow(oneRow, out);

        if(exportedCount%50==0){
            emit recordsExported(exportedCount);
        }
    }

    if(fetchToEnd && endRow==-1){
        rs->beginFetchRows();
        while(rs->moveNext()){
            for(int k=startColumn; k<=endColumn; ++k){
                oneRow[k-startColumn] = rs->getAsString(k+1);
            }

            ++exportedCount;
            exporter->exportRow(oneRow, out);
            if(exportedCount%50==0){
                emit recordsExported(exportedCount);
            }
        }
        rs->endFetchRows();
    }

    emit recordsExported(exportedCount);
}
