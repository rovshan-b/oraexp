#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "connection_page/connectionpagewizard.h"

class DataImporterFirstPage;
class DataImporterCsvOptionsPage;
class DataImporterColumnMappingsPage;
class DataImporterQueriesPage;
class DataImporterReadyPage;
class DataImporterImportPage;
class DataImporterWorksheetPage;
class QStandardItemModel;
class CsvImporter;
class ColumnMapping;

class DataImporter : public ConnectionPageWizard
{
    Q_OBJECT
public:
    explicit DataImporter(
            const QString &schemaName,
            const QString &tableName,
            DbUiManager *uiManager,
            QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);

    QStandardItemModel *getDataPreviewModel() const;
    CsvImporter *getImporter() const;
    QList<ColumnMapping*> getColumnMappings() const;
    
private:
    QString schemaName;
    QString tableName;

    DataImporterFirstPage *firstPage;
    DataImporterCsvOptionsPage *csvOptionsPage;
    DataImporterColumnMappingsPage *mappingsPage;
    DataImporterQueriesPage *queriesPage;
    DataImporterReadyPage *readyPage;
    DataImporterImportPage *importPage;
    DataImporterWorksheetPage *worksheetPage;
    
};

#endif // DATAIMPORTER_H
