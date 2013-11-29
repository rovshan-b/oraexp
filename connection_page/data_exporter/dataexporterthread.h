#ifndef DATAEXPORTERTHREAD_H
#define DATAEXPORTERTHREAD_H

#include "util/stopablethread.h"
#include <QStringList>
#include <QTextStream>
#include <QSharedPointer>
#include "beans/resultsetcolumnmetadata.h"
#include "beans/tableinfofordatacomparison.h"
#include "dataexporterobject.h"

class Statement;
class Resultset;
class DataExporterBase;
class DataExporterOptions;
class IQueryScheduler;

class DataExporterThread : public StopableThread
{
    Q_OBJECT
public:
    explicit DataExporterThread(DataExporterBase *exporter,
                                QList<QStringList> alreadyFetchedData,
                                Resultset *rs,
                                bool fetchToEnd,
                                bool autoDestroy,
                                QObject *parent);

    virtual ~DataExporterThread();
    
    void run();

    virtual void stop();

    void setOptions(IQueryScheduler *queryScheduler,
                    const QString &schemaName, const QString &tableName,
                    DataExporterOptions *options, const TableInfoForDataComparison &tableOptions);

signals:
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);

private:
    DataExporterObject worker;
};

#endif // DATAEXPORTERTHREAD_H
