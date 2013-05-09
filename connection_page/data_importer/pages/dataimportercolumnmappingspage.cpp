#include "dataimportercolumnmappingspage.h"
#include "widgets/datatable.h"
#include "models/dataimportercolumnmappingsmodel.h"
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
    mappingsModel = new DataImporterColumnMappingsModel(QStringList() << tr("Column") << tr("File field") << tr("Format"));
    mappingsModel->setColumnEnabled(DataImporterColumnMappingsModel::ColumnName, false);
    mappingsTable->setModel(mappingsModel);
    mappingsTable->horizontalHeader()->setDefaultSectionSize(170);

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

    if(currentSchemaName!=field("schemaName").toString() || currentTableName!=field("tableName").toString()){
        currentSchemaName = field("schemaName").toString().toUpper().trimmed();
        currentTableName = field("tableName").toString().toUpper().trimmed();

        Q_ASSERT(querySheduler);
        mappingsModel->removeRows(0, mappingsModel->rowCount());
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

void DataImporterColumnMappingsPage::columnsQueryCompleted(const QueryResult &result)
{
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
    mappingsModel->setData(colNameIndex, fetchResult.colValue("COLUMN_NAME"));
    mappingsModel->setData(colNameIndex, IconUtil::getIcon(fetchResult.colValue("IS_PK")=="1" ? "column_pk" : "column"), Qt::DecorationRole);
}

void DataImporterColumnMappingsPage::columnFetchCompleted(const QString &)
{
}
