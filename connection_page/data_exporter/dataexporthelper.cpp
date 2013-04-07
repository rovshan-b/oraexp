#include "dataexporthelper.h"
#include "dataexporterthread.h"
#include "beans/dataexporteroptions.h"
#include "exporters/insertexporter.h"

DataExportHelper::DataExportHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                   const QString &targetSchema, IQueryScheduler *targetScheduler,
                                   DbTreeModel *model, DataOperationOptions *options,
                                   const QHash<QString, TableInfoForDataComparison> &tableOptions,
                                   QObject *parent) :
    DataOperationHelper(sourceSchema, sourceScheduler, targetSchema, targetScheduler,
                        model, options, tableOptions, parent)
{
}

void DataExportHelper::startWorkerThread(const QString &tableName)
{
    Q_ASSERT(workerThread==0);

    DataExporterOptions *exporterOptions = static_cast<DataExporterOptions*>(options);
    TableInfoForDataComparison tabOptions = tableOptions.value(tableName);

    InsertExporter *insertExporter = dynamic_cast<InsertExporter*>(exporterOptions->exporter);
    if(insertExporter){
        if(insertExporter->schemaName.isEmpty()){
            insertExporter->schemaName = sourceSchema;
        }
        insertExporter->tableName = tabOptions.targetTableName.isEmpty() ? tableName : tabOptions.targetTableName;
    }

    exporterOptions->exporter->reset();
    DataExporterThread *exporterThread = new DataExporterThread(exporterOptions->exporter,
                                          QList<QStringList>(),
                                          0,
                                          true,
                                          false,
                                          this);

    connect(exporterThread, SIGNAL(recordsExported(int)), this, SLOT(recordsExported(int)));
    connect(exporterThread, SIGNAL(exportComplete()), this, SLOT(tableDataExported()));
    connect(exporterThread, SIGNAL(exportError(QString)), this, SLOT(exportError(QString)));

    exporterThread->setOptions(sourceScheduler, sourceSchema, tableName, exporterOptions, tabOptions);

    workerThread = exporterThread;
    workerThread->start();

    emit compareInfoAvailable(DataCompareInfo(this->currentTableName, tr("Exporting data")));
}

void DataExportHelper::recordsExported(int count)
{
    emit compareInfoAvailable(DataCompareInfo(this->currentTableName, "", count));
    emit statusChanged(QString("Exporting data for %1 - %2 records exported").arg(currentTableName).arg(count));
}

void DataExportHelper::tableDataExported()
{
    emit compareInfoAvailable(DataCompareInfo(this->currentTableName, tr("Completed")));
    tableComparisonCompleted();
}

void DataExportHelper::exportError(const QString &errorMessage)
{
    tableComparisonError("data_export", OciException(errorMessage));
}
