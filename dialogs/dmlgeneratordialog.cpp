#include "dmlgeneratordialog.h"
#include "util/dialoghelper.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "util/iconutil.h"
#include "codeeditor/codeeditor.h"
#include "widgets/checkedlistboxwidget.h"
#include "code_generators/dml/tabledmlgenerator.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/worksheet/worksheet.h"
#include <QtGui>

DmlGeneratorDialog::DmlGeneratorDialog(DbUiManager *uiManager,
                                       const QString &schemaName,
                                       const QString &tableName,
                                       OraExp::DmlType dmlType, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    schemaName(schemaName),
    tableName(tableName),
    initialDmlType(dmlType)
{

}

void DmlGeneratorDialog::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *columnsLayout = new QHBoxLayout();

    columnsList = new CheckedListBoxWidget();
    columnsLayout->addWidget(WidgetHelper::createGroupBox(columnsList, tr("Columns")));

    whereColumnsList = new CheckedListBoxWidget();
    columnsLayout->addWidget(WidgetHelper::createGroupBox(whereColumnsList, tr("Where")));

    mainLayout->addLayout(columnsLayout);

    QFormLayout *form = new QFormLayout();

    dmlTypeCombo = new QComboBox();
    dmlTypeCombo->addItem(tr("Select"), OraExp::DmlTypeSelect);
    dmlTypeCombo->addItem(tr("Insert"), OraExp::DmlTypeInsert);
    dmlTypeCombo->addItem(tr("Update"), OraExp::DmlTypeUpdate);
    dmlTypeCombo->addItem(tr("Delete"), OraExp::DmlTypeDelete);
    dmlTypeCombo->setCurrentIndex((int)initialDmlType);
    form->addRow(tr("Type"), dmlTypeCombo);

    bindStyleCombo = new QComboBox();
    bindStyleCombo->addItem(tr("Named"));
    bindStyleCombo->addItem(tr("Positioned"));
    bindStyleCombo->addItem(tr("Java"));
    form->addRow(tr("Bind style"), bindStyleCombo);

    includeSchemaCheckBox = new QCheckBox();
    form->addRow(tr("Include schema"), includeSchemaCheckBox);

    mainLayout->addLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    copyToClipboardButton = new QPushButton(tr("Copy to clipboard"));
    buttonBox->addButton(copyToClipboardButton, QDialogButtonBox::AcceptRole);
    copyToClipboardButton->setAutoDefault(false);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    dmlTypeChanged(initialDmlType);
    connect(dmlTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(dmlTypeChanged(int)));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(acceptButtonPressed(QAbstractButton*)));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    resize(this->sizeHint());
    DialogHelper::centerWindow(this);
}

void DmlGeneratorDialog::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);

    this->enqueueQuery("get_table_columns_for_editing",
                       QList<Param*>()
                       << new Param("owner", schemaName)
                       << new Param("object_name", tableName),
                       this,
                       "get_table_columns",
                       "columnsQueryCompleted",
                       "columnFetched",
                       "columnFetchCompleted");
}

void DmlGeneratorDialog::columnsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this, tr("Error retrieving column list"),
                              result.exception.getErrorMessage());
    }
}

void DmlGeneratorDialog::columnFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this, tr("Error fetching column list"),
                              fetchResult.exception.getErrorMessage());
        return;
    }

    QString columnName = fetchResult.colValue("COLUMN_NAME");
    bool isPk = fetchResult.colValue("IS_PK")=="1";
    QPixmap pixmap = IconUtil::getIcon(DbUtil::getTableIconName(isPk));

    columnsList->addItem(pixmap, columnName, !isPk || initialDmlType==OraExp::DmlTypeSelect || initialDmlType==OraExp::DmlTypeInsert);
    whereColumnsList->addItem(pixmap, columnName, isPk);
}

void DmlGeneratorDialog::columnFetchCompleted(const QString &)
{
}

void DmlGeneratorDialog::dmlTypeChanged(int newType)
{
    OraExp::DmlType dmlType = (OraExp::DmlType)newType;

    columnsList->setEnabled(dmlType!=OraExp::DmlTypeDelete);
    whereColumnsList->setEnabled(dmlType!=OraExp::DmlTypeInsert);
}

void DmlGeneratorDialog::acceptButtonPressed(QAbstractButton *button)
{
    if(button == buttonBox->button(QDialogButtonBox::Cancel)){
        return;
    }

    QString dml = generateDml();
    if(dml.isEmpty()){
        QMessageBox::information(this, tr("No column selected"),
                                 tr("Please, select at least one column to generate DML."));
        return;
    }

    if(button==copyToClipboardButton){
        QApplication::clipboard()->setText(dml);
    }else{
        uiManager->getWorksheet()->insertText(dml);
    }

    accept();
}

QString DmlGeneratorDialog::generateDml()
{
    OraExp::DmlType dmlType = (OraExp::DmlType)dmlTypeCombo->currentIndex();

    QStringList columns = columnsList->getCheckedItemNames();
    if(columns.isEmpty() && dmlType!=OraExp::DmlTypeDelete){
        return "";
    }

    QStringList whereColumns = whereColumnsList->getCheckedItemNames();
    if(whereColumns.isEmpty() && dmlType==OraExp::DmlTypeDelete){
        return "";
    }

    OraExp::BindVarStyle bindStyle = (OraExp::BindVarStyle)bindStyleCombo->currentIndex();
    bool includeSchema = includeSchemaCheckBox->isChecked();

    QString dml;

    switch(dmlType){
    case OraExp::DmlTypeSelect:
        dml = TableDmlGenerator::generateSelectStatement(schemaName, tableName, columns, whereColumns, bindStyle, includeSchema);
        break;
    case OraExp::DmlTypeInsert:
        dml = TableDmlGenerator::generateInsertStatement(schemaName, tableName, columns, bindStyle, includeSchema);
        break;
    case OraExp::DmlTypeUpdate:
        dml = TableDmlGenerator::generateUpdateStatement(schemaName, tableName, columns, whereColumns, bindStyle, includeSchema);
        break;
    case OraExp::DmlTypeDelete:
        dml = TableDmlGenerator::generateDeleteStatement(schemaName, tableName, whereColumns, bindStyle, includeSchema);
        break;
    }

    return dml;
}
