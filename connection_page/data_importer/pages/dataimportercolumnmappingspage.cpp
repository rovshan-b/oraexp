#include "dataimportercolumnmappingspage.h"
#include "widgets/datatable.h"
#include "models/dataimportercolumnmappingsmodel.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "util/iconutil.h"
#include "../dataimporter.h"
#include <QtGui>

DataImporterColumnMappingsPage::DataImporterColumnMappingsPage(QWidget *parent) :
    QWizardPage(parent), querySheduler(0)
{
    setTitle(tr("Column mappings"));
    setSubTitle(tr("Define mappings between table columns and file fields"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mappingsTable = new DataTable();
    mappingsTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mappingsModel = new DataImporterColumnMappingsModel(QStringList() << tr("Column") << tr("File field") << tr("Date format"));
    mappingsModel->setColumnEnabled(DataImporterColumnMappingsModel::ColumnName, false);
    mappingsTable->setModel(mappingsModel);
    mappingsTable->horizontalHeader()->setDefaultSectionSize(170);
    mappingsTable->setItemDelegateForColumn(DataImporterColumnMappingsModel::FileField, new IndexBasedComboBoxDelegate(this, DataImporterColumnMappingsModel::FileField));

    previewTable = new DataTable();
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTabWidget *tab = new QTabWidget();
    tab->addTab(mappingsTable, tr("Mappings"));
    tab->addTab(previewTable, tr("Data preview"));
    mainLayout->addWidget(tab);

    setLayout(mainLayout);
}

void DataImporterColumnMappingsPage::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->querySheduler = queryScheduler;
}

void DataImporterColumnMappingsPage::initializePage()
{
    DataImporter *importerWizard = static_cast<DataImporter*>(wizard());
    previewTable->setModel(importerWizard->getDataPreviewModel());
    previewTable->resizeColumnsToFitContents();

    setFileFieldList(importerWizard->getDataPreviewModel());
    loadColumnList();
}

void DataImporterColumnMappingsPage::columnsQueryCompleted(const QueryResult &result)
{
    mappingsModel->removeRows(0, mappingsModel->rowCount());

    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving column list"), result.exception.getErrorMessage());
    }
}

void DataImporterColumnMappingsPage::columnFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching column list"), fetchResult.exception.getErrorMessage());
        return;
    }

    mappingsModel->insertRow(mappingsModel->rowCount());
    int lastRowIx = mappingsModel->rowCount() - 1;
    mappingsModel->freezeRow(lastRowIx, true);

    QModelIndex colNameIndex = mappingsModel->index(lastRowIx, DataImporterColumnMappingsModel::ColumnName);
    QString columnName = fetchResult.colValue("COLUMN_NAME");
    QString dataType = fetchResult.colValue("DATA_TYPE");

    mappingsModel->setData(colNameIndex, columnName);
    mappingsModel->setData(colNameIndex, IconUtil::getIcon(fetchResult.colValue("IS_PK")=="1" ? "column_pk" : "column"), Qt::DecorationRole);
    mappingsModel->setColumnDataType(lastRowIx, dataType);

    QStringList fileFields = mappingsModel->getList(DataImporterColumnMappingsModel::FileField);
    QRegExp colMatchRegExp(columnName, Qt::CaseInsensitive);
    int colIx = fileFields.indexOf(colMatchRegExp);
    if(colIx!=-1){
        QModelIndex fileFieldIndex = mappingsModel->index(lastRowIx, DataImporterColumnMappingsModel::FileField);
        mappingsModel->setData(fileFieldIndex, colIx, Qt::EditRole);
        mappingsModel->setData(fileFieldIndex, columnName, Qt::DisplayRole);
    }
}

void DataImporterColumnMappingsPage::columnFetchCompleted(const QString &)
{
}

void DataImporterColumnMappingsPage::loadColumnList()
{
    QString newSchemaName = field("schemaName").toString().toUpper().trimmed();
    QString newTableName = field("tableName").toString().toUpper().trimmed();
    if(currentSchemaName!=newSchemaName || currentTableName!=newTableName){
        currentSchemaName = newSchemaName;
        currentTableName = newTableName;

        Q_ASSERT(querySheduler);
        mappingsModel->removeRows(0, mappingsModel->rowCount());
        mappingsModel->ensureRowCount(1);
        mappingsModel->setData(mappingsModel->index(0,0), tr("Loading..."));
        querySheduler->enqueueQuery("get_table_columns_for_editing",
                                    QList<Param*>()
                                    << new Param("owner", currentSchemaName)
                                    << new Param("object_name", currentTableName),
                                    this,
                                    "get_table_columns",
                                    "columnsQueryCompleted",
                                    "columnFetched",
                                    "columnFetchCompleted");

    }
}

void DataImporterColumnMappingsPage::setFileFieldList(QStandardItemModel *model)
{
    QStringList list;
    list.append("");

    for(int i=0; i<model->columnCount(); ++i){
        list.append(model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    }

    QStringList existingList = mappingsModel->getList(DataImporterColumnMappingsModel::FileField);
    if(list != existingList){

        for(int i=0; i<mappingsModel->rowCount(); ++i){
            QModelIndex fileFieldIndex = mappingsModel->index(i, DataImporterColumnMappingsModel::FileField);
            QModelIndex columnFormatIndex = mappingsModel->index(i, DataImporterColumnMappingsModel::ColumnFormat);
            mappingsModel->setData(fileFieldIndex, QVariant(), Qt::EditRole);
            mappingsModel->setData(fileFieldIndex, QVariant(), Qt::DisplayRole);
            mappingsModel->setData(columnFormatIndex, QVariant());
        }

        mappingsModel->setList(DataImporterColumnMappingsModel::FileField, list);
    }
}
