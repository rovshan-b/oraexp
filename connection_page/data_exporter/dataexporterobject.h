#ifndef DATAEXPORTEROBJECT_H
#define DATAEXPORTEROBJECT_H

#include <QObject>
#include <QStringList>
#include <QTextStream>
#include <QSharedPointer>
#include "beans/resultsetcolumnmetadata.h"
#include "beans/tableinfofordatacomparison.h"

class Statement;
class Resultset;
class DataExporterBase;
class DataExporterOptions;
class IQueryScheduler;

class DataExporterObject : public QObject
{
    Q_OBJECT
public:
    explicit DataExporterObject(DataExporterBase *exporter,
                                QList<QStringList> alreadyFetchedData,
                                Resultset *rs,
                                bool fetchToEnd,
                                QObject *parent = 0);

    virtual ~DataExporterObject();

    void exportData();

    void stop();

    void setOptions(IQueryScheduler *queryScheduler,
                    const QString &schemaName, const QString &tableName,
                    DataExporterOptions *options, const TableInfoForDataComparison &tableOptions);
    
signals:
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);

private:
    bool stopped;
    DataExporterBase *exporter;
    QList<QStringList> alreadyFetchedData;
    Resultset *rs;
    bool fetchToEnd;

    bool bulkMode;
    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString tableName;
    DataExporterOptions *options;
    TableInfoForDataComparison tableOptions;

    Statement *getStatement();
    void exportToStream(QTextStream &out);
    
};

#endif // DATAEXPORTEROBJECT_H
