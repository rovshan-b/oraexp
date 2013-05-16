#include "dataimportercolumnmappingspage.h"
#include "widgets/datatable.h"
#include "models/dataimportercolumnmappingsmodel.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "../dataimporter.h"
#include "../columnmapping.h"
#include <QtGui>

DataImporterColumnMappingsPage::DataImporterColumnMappingsPage(QWidget *parent) :
    QWizardPage(parent), querySheduler(0), loadInProgress(true)
{
    setTitle(tr("Column mappings"));
    setSubTitle(tr("Define mappings between table columns and file fields"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mappingsTable = new DataTable();
    mappingsTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mappingsModel = new DataImporterColumnMappingsModel(QStringList() << tr("Column") << tr("File field") << tr("Date format"), this);
    mappingsModel->setColumnEnabled(DataImporterColumnMappingsModel::ColumnName, false);
    mappingsTable->setModel(mappingsModel);
    mappingsTable->horizontalHeader()->setDefaultSectionSize(170);

    mappingsTable->setItemDelegateForColumn(DataImporterColumnMappingsModel::FileField, new IndexBasedComboBoxDelegate(this, DataImporterColumnMappingsModel::FileField));
    mappingsTable->setItemDelegateForColumn(DataImporterColumnMappingsModel::ColumnFormat, new ComboBoxDelegate(this, DataImporterColumnMappingsModel::ColumnFormat));

    QStringList dateTimeFormats = DbUtil::getDateTimeFormats();
    dateTimeFormats.prepend("");
    mappingsModel->setList(DataImporterColumnMappingsModel::ColumnFormat, dateTimeFormats);

    previewTable = new DataTable();
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTabWidget *tab = new QTabWidget();
    tab->addTab(mappingsTable, tr("Mappings"));
    tab->addTab(previewTable, tr("Data preview"));
    mainLayout->addWidget(tab);

    setLayout(mainLayout);

    connect(mappingsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(mappingsModelDataChanged(QModelIndex,QModelIndex)));
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

bool DataImporterColumnMappingsPage::validatePage()
{
    QList<ColumnMapping *> columnMappings = getColumnMappings();

    bool isValid = columnMappings.size() > 0;

    qDeleteAll(columnMappings);

    if(!isValid){
        QMessageBox::critical(this,
                              tr("Page not valid"),
                              tr("Please, define at least one column to file field mapping"));
    }

    return isValid;
}

QList<ColumnMapping *> DataImporterColumnMappingsPage::getColumnMappings() const
{
    QList<ColumnMapping*> result;

    QList<QString> fileFieldNames = mappingsModel->getList(DataImporterColumnMappingsModel::FileField);

    for(int i=0; i<mappingsModel->rowCount(); ++i){
        int fileFieldIx = mappingsModel->data(mappingsModel->index(i, DataImporterColumnMappingsModel::FileField), Qt::EditRole).toInt();
        if(fileFieldIx <= 0){
            continue;
        }
        QString columnName = mappingsModel->data(mappingsModel->index(i, DataImporterColumnMappingsModel::ColumnName)).toString();
        QString columnDataType = mappingsModel->getColumnDataType(i);
        QString dateFormat = mappingsModel->data(mappingsModel->index(i, DataImporterColumnMappingsModel::ColumnFormat)).toString();

        ColumnMapping *mapping = new ColumnMapping();

        mapping->columnName = columnName;
        mapping->columnDataType = columnDataType;
        mapping->fileFieldIx = fileFieldIx - 1;
        mapping->fileFieldName = fileFieldNames.at(fileFieldIx);
        mapping->dateFormat = dateFormat;

        result.append(mapping);
    }

    return result;
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
    int fileFieldIx = fileFields.indexOf(colMatchRegExp);
    if(fileFieldIx!=-1){
        QModelIndex fileFieldIndex = mappingsModel->index(lastRowIx, DataImporterColumnMappingsModel::FileField);
        mappingsModel->setData(fileFieldIndex, fileFieldIx, Qt::EditRole);
        mappingsModel->setData(fileFieldIndex, columnName, Qt::DisplayRole);
    }
    mappingsModel->setDateFormat(lastRowIx, getNonEmptyDates(fileFieldIx - 1)); //colIx is 1 based, because first item is always empty
}

void DataImporterColumnMappingsPage::columnFetchCompleted(const QString &)
{
    this->loadInProgress = false;
    mappingsTable->resizeColumnToContents(DataImporterColumnMappingsModel::ColumnName);
}

void DataImporterColumnMappingsPage::mappingsModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(loadInProgress){
        return;
    }

    for(int i=topLeft.row(); i<=bottomRight.row(); ++i){
        for(int k=topLeft.column(); k<=bottomRight.column(); ++k){
            if(k!=DataImporterColumnMappingsModel::FileField){
                continue;
            }
            int fileFieldIx = mappingsModel->data(mappingsModel->index(i, DataImporterColumnMappingsModel::FileField), Qt::EditRole).toInt();
            if(fileFieldIx <= 0){
                mappingsModel->setData(mappingsModel->index(i, DataImporterColumnMappingsModel::ColumnFormat), "");
            }else{
                mappingsModel->setDateFormat(i, getNonEmptyDates(fileFieldIx - 1));
            }
        }
    }
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

        this->loadInProgress = true;

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
            mappingsModel->setData(fileFieldIndex, QVariant(), Qt::EditRole);
            mappingsModel->setData(fileFieldIndex, QVariant(), Qt::DisplayRole);
        }

        mappingsModel->setList(DataImporterColumnMappingsModel::FileField, list);
    }
}

QStringList DataImporterColumnMappingsPage::getNonEmptyDates(int previewTableColIx) const
{
    QStringList results;

    QAbstractItemModel *model = previewTable->model();
    for(int i=0; i<model->rowCount(); ++i){
        QString date = model->index(i, previewTableColIx).data().toString().trimmed();
        if(!date.isEmpty()){
            results.append(date);
        }
    }

    return results;
}
