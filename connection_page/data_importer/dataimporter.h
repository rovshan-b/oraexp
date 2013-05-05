#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "connection_page/connectionpagewizard.h"

class DataImporterFirstPage;
class DataImporterCsvOptionsPage;

class DataImporter : public ConnectionPageWizard
{
    Q_OBJECT
public:
    explicit DataImporter(
            const QString &schemaName,
            const QString &tableName,
            QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);
    
private:
    QString schemaName;
    QString tableName;

    DataImporterFirstPage *firstPage;
    DataImporterCsvOptionsPage *csvOptionsPage;
    
};

#endif // DATAIMPORTER_H
