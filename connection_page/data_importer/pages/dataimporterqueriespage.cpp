#include "dataimporterqueriespage.h"
#include "codeeditor/codeeditor.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "../dataimporter.h"
#include "../columnmapping.h"
#include "code_generators/dml/tabledmlgenerator.h"
#include <QtGui>

DataImporterQueriesPage::DataImporterQueriesPage(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWizardPage(uiManager, parent)
{
    setTitle(tr("Queries"));
    setSubTitle(tr("Review and customize queries used to import data"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *infoLabel = new QLabel();
    infoLabel->setWordWrap(true);
    infoLabel->setText(tr("You can use PL/SQL anonymous block as before and after import query to perform multiple operations. "
                          "Import query must be kept as DML. Bind variable names used in import query must match file field names defined in Column mappings page."));
    mainLayout->addWidget(infoLabel);

    createBeforeImportQueryBox(mainLayout);
    createImportQueryBox(mainLayout);
    createAfterImportQueryBox(mainLayout);

    setLayout(mainLayout);

    registerField("beforeImportGroupBox", beforeImportGroupBox, "checked");
    registerField("beforeImportQuery", beforeImportQueryEditor, "plainText");

    registerField("importQuery", importQueryEditor, "plainText");

    registerField("afterImportGroupBox", afterImportGroupBox, "checked");
    registerField("afterImportQuery", afterImportQueryEditor, "plainText");
}

void DataImporterQueriesPage::initializePage()
{
    QString schemaName = field("schemaName").toString().trimmed().toUpper();
    QString tableName = field("tableName").toString().trimmed().toUpper();

    bool tableNameChanged = false;

    if(schemaName != currentSchemaName || tableName != currentTableName){
        tableNameChanged = true;
        this->currentSchemaName = schemaName;
        this->currentTableName = tableName;
    }

    if(beforeImportQueryEditor->toPlainText().isEmpty() || tableNameChanged){
        QString deleteDml = TableDmlGenerator::generateDeleteStatement(schemaName, tableName);
        beforeImportQueryEditor->setPlainText(deleteDml);
    }


    DataImporter *importerWizard = static_cast<DataImporter*>(wizard());
    QList<ColumnMapping*> columnMappings = importerWizard->getColumnMappings();

    QString insertDml = generateInsertStatement(schemaName, tableName, columnMappings);
    importQueryEditor->setPlainText(insertDml);

    qDeleteAll(columnMappings);

}

void DataImporterQueriesPage::cleanupPage()
{
}

bool DataImporterQueriesPage::validatePage()
{
    if(importQueryEditor->toPlainText().trimmed().isEmpty()){
        QMessageBox::critical(this, tr("Page not valid"),
                              tr("Please, enter import query"));
    }

    return true;
}

void DataImporterQueriesPage::createBeforeImportQueryBox(QVBoxLayout *layout)
{
    QVBoxLayout *boxLayout = new QVBoxLayout();

    beforeImportQueryEditor = new CodeEditor();
    boxLayout->addWidget(beforeImportQueryEditor);

    beforeImportGroupBox = WidgetHelper::createGroupBox(boxLayout, tr("Before import query"));
    beforeImportGroupBox->setCheckable(true);
    beforeImportGroupBox->setChecked(false);

    layout->addWidget(beforeImportGroupBox);
}

void DataImporterQueriesPage::createImportQueryBox(QVBoxLayout *layout)
{
    QVBoxLayout *boxLayout = new QVBoxLayout();

    importQueryEditor = new CodeEditor();
    boxLayout->addWidget(importQueryEditor);

    QGroupBox *importGroupBox = WidgetHelper::createGroupBox(boxLayout, tr("Import query"));

    layout->addWidget(importGroupBox);

}

void DataImporterQueriesPage::createAfterImportQueryBox(QVBoxLayout *layout)
{
    QVBoxLayout *boxLayout = new QVBoxLayout();

    afterImportQueryEditor = new CodeEditor();
    boxLayout->addWidget(afterImportQueryEditor);

    afterImportGroupBox = WidgetHelper::createGroupBox(boxLayout, tr("After import query"));
    afterImportGroupBox->setCheckable(true);
    afterImportGroupBox->setChecked(true);

    layout->addWidget(afterImportGroupBox);

    afterImportQueryEditor->setPlainText("COMMIT");
}

QString DataImporterQueriesPage::generateInsertStatement(const QString &schemaName,
                                                         const QString &tableName,
                                                         const QList<ColumnMapping *> &columnMappings)
{
    QString dml = QString("INSERT INTO \"%1\".\"%2\" ").arg(schemaName, tableName);

    QStringList columnNames;
    QStringList values;

    for(int i=0; i<columnMappings.size(); ++i){
        ColumnMapping *mapping = columnMappings.at(i);
        columnNames.append(mapping->columnName);

        QString value = QString(":%1").arg(mapping->fileFieldName);
        if(!mapping->dateFormat.isEmpty()){
            value = DbUtil::toDateOrIntervalOrTimestamp(value, mapping->columnDataType, mapping->dateFormat);
        }

        values.append(value);
    }

    dml.append(QString("(%1)\nVALUES (%2)").arg(joinEnclosed(columnNames, ", "), values.join(", ")));

    return dml;
}
