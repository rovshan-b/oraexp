#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "connection_page/connectionpagetab.h"

class DbItemListComboBox;

class DataImporter : public ConnectionPageTab
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
    
private:
    QString schemaName;
    QString tableName;

    DbItemListComboBox *schemaList;
    DbItemListComboBox *tableList;


    void loadTableList();
    QString getSchemaName() const;
    
};

#endif // DATAIMPORTER_H
