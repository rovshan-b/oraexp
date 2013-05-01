#include "dataimporter.h"
#include "widgets/dbitemlistcombobox.h"
#include <QtGui>

DataImporter::DataImporter(const QString &schemaName, const QString &tableName, DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    tableName(tableName)
{
}

void DataImporter::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    schemaList->loadItems(this, "get_schema_list");
    loadTableList();

    emitInitCompletedSignal();
}

void DataImporter::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();

    schemaList = new DbItemListComboBox(schemaName, "user", true, false);
    form->addRow(tr("Schema"), schemaList);

    tableList = new DbItemListComboBox(tableName, "table", true, false);
    form->addRow(tr("Table"), tableList);

    mainLayout->addLayout(form);

    setContentsMargins(2,2,2,2);
    setLayout(mainLayout);
}

void DataImporter::loadTableList()
{
    tableList->loadItems(this, "get_table_list", QList<Param*>() << new Param(":owner", getSchemaName()) << new Param(":object_name", QString("")));
}

QString DataImporter::getSchemaName() const
{
    return schemaList->currentText().toUpper().trimmed();
}
