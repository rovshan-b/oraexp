#include "tablecreatorcolumns.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/datatypecomboboxdelegate.h"
#include "delegates/booleandelegate.h"
#include "delegates/identifiernamedelegate.h"
#include "delegates/plaintexteditordelegate.h"
#include "delegates/templatedparamsdelegate.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "dialogs/lobparamsdialog.h"
#include "connectivity/dbconnection.h"
#include "../tablecreatortabs.h"
#include "util/itemcreatorhelper.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

#include <QDebug>

TableCreatorColumns::TableCreatorColumns(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    TableCreatorTabWithTableView(tableCreator, editMode, parent), originalColumnList(0)
{

}

void TableCreatorColumns::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    TableCreatorTabWithTableView::setQueryScheduler(queryScheduler);

    customizeTableWidget();

    //if(isEditMode()){
    //    loadColumnInfo();
    //}
}

void TableCreatorColumns::customizeTableWidget()
{
    QStringList columnNames;
    columnNames.append(tr("Name"));
    columnNames.append(tr("Column type"));
    columnNames.append(tr("Data type"));
    columnNames.append(tr("Virtual definition"));
    columnNames.append(tr("Not null"));
    columnNames.append(tr("Default"));
    columnNames.append(tr("Primary key"));
    columnNames.append(tr("Unique"));
    columnNames.append(tr("LOB properties"));
    columnNames.append(tr("Comments"));

    TableColumnsModel *tableModel=new TableColumnsModel(columnNames, this);
    table->setModel(tableModel);

    table->setColumnWidth(TableColumnsModel::ColumnName, 150);
    table->setColumnWidth(TableColumnsModel::ColumnColumnType, 130);
    table->setColumnWidth(TableColumnsModel::ColumnDataType, 150);
    table->setColumnWidth(TableColumnsModel::ColumnNotNull, 80);
    table->setColumnWidth(TableColumnsModel::ColumnVirtualDefinition, 150);
    table->setColumnWidth(TableColumnsModel::ColumnLobProperties, 150);
    table->setColumnWidth(TableColumnsModel::ColumnComments, 200);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    IdentifierNameDelegate *colNameDelegate=new IdentifierNameDelegate(this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnName, colNameDelegate);

    QStringList columnTypes;
    columnTypes.append(tr("Normal"));
    columnTypes.append(tr("Virtual"));
    IndexBasedComboBoxDelegate *columnTypeDelegate=new IndexBasedComboBoxDelegate(QIcon(), columnTypes, this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnColumnType, columnTypeDelegate);

    DataTypeComboBoxDelegate *dataTypeDelegate=new DataTypeComboBoxDelegate(this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnDataType, dataTypeDelegate);

    BooleanDelegate *isNullableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnNotNull, isNullableDelegate);

    BooleanDelegate *isPrimaryKeyDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnPrimaryKey, isPrimaryKeyDelegate);

    BooleanDelegate *isUniqueDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnUnique, isUniqueDelegate);

    TemplatedParamsDelegate<LobParams, LobParamsDialog> *lobDelegate=new TemplatedParamsDelegate<LobParams, LobParamsDialog>(this->queryScheduler, this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnLobProperties, lobDelegate);

    PlainTextEditorDelegate *virtualDefinitionDelegate=new PlainTextEditorDelegate(tr("Edit virtual column definition"), this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnVirtualDefinition, virtualDefinitionDelegate);

    PlainTextEditorDelegate *commentsDelegate=new PlainTextEditorDelegate(tr("Edit column comments"), this);
    table->setItemDelegateForColumn(TableColumnsModel::ColumnComments, commentsDelegate);

    //table->verticalHeader()->setVisible(false);
    table->model()->insertRows(0, 1);

    showAdvancedOptions(false);

    if(isEditMode()){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void TableCreatorColumns::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    table->setColumnHidden(TableColumnsModel::ColumnColumnType, this->queryScheduler->getDb()->supportsVirtualColumns() ? !show : true);
    table->setColumnHidden(TableColumnsModel::ColumnLobProperties, !show);
    table->setColumnHidden(TableColumnsModel::ColumnVirtualDefinition, this->queryScheduler->getDb()->supportsVirtualColumns() ? !show : true);
    table->setUpdatesEnabled(true);
}

QStringList TableCreatorColumns::getStringList(const QString &)
{
    QStringList result;

    QAbstractItemModel *tableModel=table->model();
    int rowCount=tableModel->rowCount();

    for(int i=0; i<rowCount; ++i){
        QString columnName=tableModel->index(i, 0).data().toString();
        if(columnName.isEmpty()){
            break;
        }

        result.append(columnName);
    }

    return result;
}

void TableCreatorColumns::setTableType(OraExp::TableType tableType)
{
    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());
    model->setTableType(tableType);
}

void TableCreatorColumns::setTableInfo(TableInfo *tableInfo)
{
    originalColumnList=&tableInfo->columns;
    if(originalColumnList->size()>0){
        populateTableWithColumnList();
    }
}


void TableCreatorColumns::saveLobParamsToModel(TableColumnsModel *model, int rowIx, const LobParams &lobParams)
{
    model->setData(model->index(rowIx, TableColumnsModel::ColumnLobProperties),
                            QVariant::fromValue(lobParams), Qt::UserRole);

    model->setData(model->index(rowIx, TableColumnsModel::ColumnLobProperties),
                            QVariant::fromValue(lobParams), Qt::UserRole+1); //original settings
}

void TableCreatorColumns::populateTableWithColumnList()
{
    Q_ASSERT(originalColumnList);

    table->setUpdatesEnabled(false);

    int colCount=originalColumnList->count();

    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());
    model->ensureRowCount(colCount);

    ColumnInfo colInfo;
    for(int i=0; i<originalColumnList->count(); ++i){
        colInfo = originalColumnList->at(i);

        model->setData(model->index(i, TableColumnsModel::ColumnName), colInfo.name);
        model->setData(model->index(i, TableColumnsModel::ColumnColumnType), (int)colInfo.columnType);
        model->setData(model->index(i, TableColumnsModel::ColumnDataType), colInfo.dataType);
        model->setData(model->index(i, TableColumnsModel::ColumnVirtualDefinition), colInfo.virtualDefinition);
        model->setData(model->index(i, TableColumnsModel::ColumnNotNull), !colInfo.isNullable, Qt::EditRole);
        model->setData(model->index(i, TableColumnsModel::ColumnDefault), colInfo.defaultValue);
        model->setData(model->index(i, TableColumnsModel::ColumnPrimaryKey), colInfo.isPrimaryKey, Qt::EditRole);
        model->setData(model->index(i, TableColumnsModel::ColumnUnique), colInfo.isUnique, Qt::EditRole);
        model->setData(model->index(i, TableColumnsModel::ColumnComments), colInfo.comments);

        if(!colInfo.lobParams.isEmpty){
            saveLobParamsToModel(model, i, colInfo.lobParams);
        }
    }

    table->resizeColumnAccountingForEditor(TableColumnsModel::ColumnName);
    table->resizeColumnAccountingForEditor(TableColumnsModel::ColumnVirtualDefinition);
    table->resizeColumnToContents(TableColumnsModel::ColumnPrimaryKey);
    table->resizeColumnToContents(TableColumnsModel::ColumnUnique);
    table->resizeColumnAccountingForEditor(TableColumnsModel::ColumnComments);

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
    disableColumnsForFrozenRows(lastRowIx);
}

void TableCreatorColumns::disableColumnsForFrozenRows(int rowIx)
{
    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());

    model->setColumnEnabled(TableColumnsModel::ColumnColumnType, false, rowIx);
    model->setColumnEnabled(TableColumnsModel::ColumnPrimaryKey, false, rowIx);
    model->setColumnEnabled(TableColumnsModel::ColumnUnique, false, rowIx);

    if(model->getTableType()==OraExp::TableTypeExternal){
        model->setColumnEnabled(TableColumnsModel::ColumnName, false, rowIx);
    }
}

QList<ColumnInfo> TableCreatorColumns::getColumnsInfo() const
{
    QList<ColumnInfo> results;

    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());
    int rowCount=model->rowCount();

    ColumnInfo colInfo;

    for(int i=0; i<rowCount; ++i){
        colInfo=model->itemInfoFromModelRow(i);
        if(colInfo.columnId==-1){
            continue;
        }

        results.append(colInfo);
    }

    return results;
}

void TableCreatorColumns::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalColumnList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("add_")){
        Q_ASSERT(originalColumnList->size()>rowIx);
    }

    ColumnInfo modifiedColInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("add_table_column_")){

        originalColumnList->append(modifiedColInfo);
        model->freezeRow(rowIx);
        disableColumnsForFrozenRows(rowIx);

        saveLobParamsToModel(model, rowIx, modifiedColInfo.lobParams);

    }else if(taskName.startsWith("add_column_comments_") ||
             taskName.startsWith("alter_column_comments_")){

        (*originalColumnList)[rowIx].comments=modifiedColInfo.comments;

    }else if(taskName.startsWith("drop_table_column_")){

        (*originalColumnList)[rowIx].dropped=true;

    }else if(taskName.startsWith("rename_table_column_")){

        (*originalColumnList)[rowIx].name=modifiedColInfo.name;

    }else if(taskName.startsWith("alter_column_data_type_")){

        (*originalColumnList)[rowIx].dataType=modifiedColInfo.dataType;

    }else if(taskName.startsWith("alter_column_nullable_")){

        (*originalColumnList)[rowIx].isNullable=modifiedColInfo.isNullable;

    }else if(taskName.startsWith("alter_column_default_value_")){

        (*originalColumnList)[rowIx].defaultValue=modifiedColInfo.defaultValue;

    }else if(taskName.startsWith("alter_column_virtual_definition_")){

        (*originalColumnList)[rowIx].virtualDefinition=modifiedColInfo.virtualDefinition;

    }else if(taskName.startsWith("alter_column_lob_storage_")){

        (*originalColumnList)[rowIx].lobParams=modifiedColInfo.lobParams;
        saveLobParamsToModel(model, rowIx, modifiedColInfo.lobParams);

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalColumnList);
}

void TableCreatorColumns::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TableColumnsModel *model=static_cast<TableColumnsModel*>(table->model());

    ItemCreatorHelper::handleTableDataChanged(model, originalColumnList, from, to);
}
