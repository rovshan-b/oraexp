#include "dataexporterthread.h"

DataExporterThread::DataExporterThread(DataExporterBase *exporter,
                                       QList<QStringList> alreadyFetchedData,
                                       Resultset *rs, bool fetchToEnd,
                                       bool autoDestroy, QObject *parent) :
    StopableThread(parent),
    worker(exporter, alreadyFetchedData, rs, fetchToEnd, 0)
{
    if(autoDestroy){
        connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    }

    connect(&worker, SIGNAL(recordsExported(int)), this, SIGNAL(recordsExported(int)));
    connect(&worker, SIGNAL(exportComplete()), this, SIGNAL(exportComplete()));
    connect(&worker, SIGNAL(exportError(QString)), this, SIGNAL(exportError(QString)));
}

DataExporterThread::~DataExporterThread()
{
}

void DataExporterThread::run()
{
    worker.exportData();
}

void DataExporterThread::stop()
{
    StopableThread::stop();
    worker.stop();
}

void DataExporterThread::setOptions(IQueryScheduler *queryScheduler,
                                    const QString &schemaName,
                                    const QString &tableName,
                                    DataExporterOptions *options,
                                    const TableInfoForDataComparison &tableOptions)
{
    worker.setOptions(queryScheduler, schemaName, tableName, options, tableOptions);
}
