#include "dataimporterfirstpage.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/fileselectorwidget.h"
#include "util/strutil.h"
#include "util/filesystemutil.h"
#include <QtGui>

DataImporterFirstPage::DataImporterFirstPage(const QString &schemaName, const QString &tableName, QWidget *parent) :
    ConnectionPageWizardPage(parent)
{
    setTitle(tr("Table & File selection"));
    setSubTitle(tr("Select table and file name to import data from"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();

    schemaList = new DbItemListComboBox(schemaName, "user", true, false);
    form->addRow(tr("Schema"), schemaList);

    tableList = new DbItemListComboBox(tableName, "table", true, false);
    form->addRow(tr("Table"), tableList);

    fileSelector = new FileSelectorWidget(FileSelectorWidget::Open);
    form->addRow(tr("Filename"), fileSelector);

    mainLayout->addLayout(form);

    setLayout(mainLayout);

    registerField("schemaName", schemaList->lineEdit());
    registerField("tableName", tableList->lineEdit());
    registerField("fileName", fileSelector->lineEdit());

    connect(schemaList->lineEdit(), SIGNAL(editingFinished()), this, SLOT(loadTableList()));
}

void DataImporterFirstPage::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    schemaList->loadItems(queryScheduler, "get_schema_list");
    loadTableList();
}

bool DataImporterFirstPage::validatePage()
{
    QString message;

    if(schemaList->lineEdit()->text().trimmed().isEmpty()){
        message.append(tr("  - Schema name must be entered"));
    }

    if(tableList->lineEdit()->text().trimmed().isEmpty()){
        addEOL(message);
        message.append(tr("  - Table name must be entered"));
    }

    QString filename=fileSelector->fileName();
    if(filename.isEmpty()){
        addEOL(message);
        message.append(tr("  - Filename name must be entered"));
    }else{
        QString errorMessage;
        bool canRead = FileSystemUtil::tryRead(filename, &errorMessage);
        if(!canRead){
            addEOL(message);
            message.append("  - ").append(errorMessage);
        }
    }


    if(!message.isEmpty()){
        QMessageBox::critical(this,
                              tr("Page not valid"),
                              tr("Please, correct following errors and try again:\n%1").arg(message));
        return false;
    }

    return true;
}

void DataImporterFirstPage::loadTableList()
{
    QString schemaName = getSchemaName();
    if(schemaName == lastLoadedSchemaName){
        return;
    }

    lastLoadedSchemaName = schemaName;

    tableList->loadItems(this->queryScheduler,
                         "get_table_list",
                         QList<Param*>() <<
                            new Param(":owner", schemaName) <<
                            new Param(":object_name",
                         QString("")));
}

QString DataImporterFirstPage::getSchemaName() const
{
    return schemaList->currentText().toUpper().trimmed();
}
