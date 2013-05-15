#ifndef DATAIMPORTERTHREAD_H
#define DATAIMPORTERTHREAD_H

#include "connection_page/data_operation/dataoperationthread.h"
#include "interfaces/idataimportlistener.h"

class ColumnMapping;
class CsvImporter;

class DataImporterThread : public DataOperationThread, public IDataImportListener
{
    Q_OBJECT
public:
    explicit DataImporterThread(const QString &sourceSchema,
                                DbConnection *sourceDb,
                                const QString &tableName,
                                const QList<ColumnMapping*> &columnMappings,
                                const QString &beforeImportQuery,
                                const QString &insertQuery,
                                const QString &afterImportQuery,
                                CsvImporter *importer,
                                QObject *parent);

    virtual ~DataImporterThread();

    virtual void headerAvailable(const QStringList &headerTitles);
    virtual void rowAvailable(const QStringList &values);

signals:
    void chunkImported(int chunkSize);

protected:
    virtual void run();

    void prepareBindArrayForColumn(const QString &, const QString &dataType, int length, int);
    
private:
    QList<ColumnMapping*> columnMappings;
    QString beforeImportQuery;
    QString insertQuery;
    QString afterImportQuery;
    CsvImporter *importer;

    BulkOperationHelper bulkHelper;
    Statement *stmt;

    QString currentTaskName;

    void importData();

    int currentOffset;
    
};

#endif // DATAIMPORTERTHREAD_H
