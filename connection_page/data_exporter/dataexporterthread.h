#ifndef DATAEXPORTERTHREAD_H
#define DATAEXPORTERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QTextStream>
#include <QSharedPointer>
#include "beans/resultsetcolumnmetadata.h"

class Resultset;
class DataExporterBase;

class DataExporterThread : public QThread
{
    Q_OBJECT
public:
    explicit DataExporterThread(DataExporterBase *exporter,
                                QSharedPointer<ResultsetColumnMetadata> columnMetadata,
                                QList<QStringList> alreadyFetchedData,
                                Resultset *rs,
                                bool fetchToEnd,
                                QObject *parent);

    virtual ~DataExporterThread();
    
    void run();

signals:
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);

private:
    DataExporterBase *exporter;
    QSharedPointer<ResultsetColumnMetadata> columnMetadata;
    QList<QStringList> alreadyFetchedData;
    Resultset *rs;
    bool fetchToEnd;

    void exportToStream(QTextStream &out);
    
};

#endif // DATAEXPORTERTHREAD_H
