#include "tablecreatorindexes.h"
#include "delegates/booleandelegate.h"
#include "delegates/indexnameeditordelegate.h"
#include "delegates/schemaselectordelegate.h"
#include "delegates/columnselectordelegate.h"
#include "delegates/numericdelegate.h"
#include "delegates/indexstorageparamsdelegate.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "delegates/partitioningparamsdelegate.h"
#include "connectivity/dbconnection.h"
#include "../tablecreatortabs.h"
#include "tablecreatorcolumns.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "util/itemcreatorhelper.h"
#include "util/dbutil.h"
#include "enums.h"
#include "tablecreatorpartitions.h"
#include "dialogs/partitioningparamsdialog.h"
#include <QtGui>

TableCreatorIndexes::TableCreatorIndexes(TableCreatorTabs* tableCreator, DbObjectCreator::CreatorMode creatorMode, QWidget *parent) :
    TableCreatorTabWithTableView(tableCreator, creatorMode, parent), originalIndexList(0)
{
}

void TableCreatorIndexes::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    TableCreatorTabWithTableView::setQueryScheduler(queryScheduler);

    customizeTableWidget(objectCreator->getSchemaName());
    //if(isEditMode()){
    //    loadIndexList();
    //}
}

void TableCreatorIndexes::customizeTableWidget(const QString &schemaName)
{
    QStringList columnNames;
    columnNames.append(tr("Owner"));
    columnNames.append(tr("Name"));
    columnNames.append(tr("Type"));
    columnNames.append(tr("Columns"));
    columnNames.append(tr("Compress"));
    columnNames.append(tr("Prefix length"));
    columnNames.append(tr("Parallel"));
    columnNames.append(tr("Parallel degree"));
    columnNames.append(tr("Reverse"));
    columnNames.append(tr("Storage"));
    columnNames.append(tr("Partitioning"));
    columnNames.append(tr("Partition definition"));

    TableIndexesModel *tableModel=new TableIndexesModel(columnNames, this);
    table->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);

    table->setColumnWidth(TableIndexesModel::IndexCompress, 80);
    table->setColumnWidth(TableIndexesModel::IndexReverse, 80);
    table->setColumnWidth(TableIndexesModel::IndexParallel, 80);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    SchemaSelectorDelegate *schemaListDelegate=new SchemaSelectorDelegate(schemaName, this->queryScheduler, this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexOwner, schemaListDelegate);

    IndexNameEditorDelegate *indexNameDelegate=new IndexNameEditorDelegate(schemaName, this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexName, indexNameDelegate);

    IndexBasedComboBoxDelegate *typeDelegate=new IndexBasedComboBoxDelegate(this, TableIndexesModel::IndexType);
    tableModel->setList(TableIndexesModel::IndexType, DbUtil::getIndexTypeNames());
    tableModel->setColumnIcon(TableIndexesModel::IndexType, IconUtil::getIcon("index"));
    table->setItemDelegateForColumn(TableIndexesModel::IndexType, typeDelegate);

    ColumnSelectorDelegate *columnsDelegate=new ColumnSelectorDelegate(objectCreator->getColumnsTab(), tr("Select columns"), this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexColumns, columnsDelegate);

    BooleanDelegate *compressDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableIndexesModel::IndexCompress, compressDelegate);

    NumericDelegate *prefixLengthDelegate=new NumericDelegate(this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexPrefixLength, prefixLengthDelegate);

    IndexBasedComboBoxDelegate *parallelDelegate=new IndexBasedComboBoxDelegate(this, TableIndexesModel::IndexParallel);
    tableModel->setList(TableIndexesModel::IndexParallel, DbUtil::getIndexParallelOptionNames());
    table->setItemDelegateForColumn(TableIndexesModel::IndexParallel, parallelDelegate);

    NumericDelegate *parallelDegreeDelegate=new NumericDelegate(this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexParallelDegree, parallelDegreeDelegate);

    BooleanDelegate *reverseDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableIndexesModel::IndexReverse, reverseDelegate);

    IndexStorageParamsDelegate *storageDelegate=new IndexStorageParamsDelegate(this->queryScheduler, this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexStorage, storageDelegate);

    ComboBoxDelegate *partitioningTypeDelegate=new ComboBoxDelegate(this, TableIndexesModel::IndexPartitioningType, false, false);
    tableModel->setList(TableIndexesModel::IndexPartitioningType, DbUtil::getIndexPartitioningTypeNames());
    table->setItemDelegateForColumn(TableIndexesModel::IndexPartitioningType, partitioningTypeDelegate);

    PartitioningParamsDelegate *partititionDefinitionDelegate=new PartitioningParamsDelegate(this->queryScheduler, true, objectCreator, this);
    table->setItemDelegateForColumn(TableIndexesModel::IndexPartitionDefinition, partititionDefinitionDelegate);

    //table->verticalHeader()->setVisible(false);
    table->model()->insertRows(0, 1);

    showAdvancedOptions(false);

    if(getCreatorMode() == DbObjectCreator::EditExisting){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void TableCreatorIndexes::setTableInfo(TableInfo *tableInfo)
{
    originalIndexList=&tableInfo->indexes;
    if(originalIndexList->size()>0){
        populateTableWithIndexList();
    }
}

void TableCreatorIndexes::savePartitionParamsToModel(int rowIx, const TablePartitioningInfo &pInfo, const QString &taskName)
{
    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());

    QVariant value;

    if(!taskName.isEmpty()){ //merge changes with original settings

        QVariant params=model->index(rowIx, TableIndexesModel::IndexPartitionDefinition).data(Qt::UserRole+1);
        if(params.isValid() && params.canConvert<TablePartitioningInfo>()){
            TablePartitioningInfo partInfoToSave = params.value<TablePartitioningInfo>();
            partInfoToSave.partitionInfo.commitChanges(taskName, pInfo.partitionInfo);
            value=QVariant::fromValue(partInfoToSave);

            model->setData(model->index(rowIx, TableIndexesModel::IndexPartitionDefinition),
                                value, Qt::UserRole+1); //original settings
        }

    }else{

        value=QVariant::fromValue(pInfo);

        model->setData(model->index(rowIx, TableIndexesModel::IndexPartitionDefinition),
                            value, Qt::UserRole);

        model->setData(model->index(rowIx, TableIndexesModel::IndexPartitionDefinition),
                            value, Qt::UserRole+1); //original settings
    }

}

void TableCreatorIndexes::populateTableWithIndexList()
{
    Q_ASSERT(originalIndexList);

    table->setUpdatesEnabled(false);

    int colCount=originalIndexList->count();

    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    model->ensureRowCount(colCount);

    IndexInfo indexInfo;
    for(int i=0; i<originalIndexList->count(); ++i){
        indexInfo = originalIndexList->at(i);

        model->setData(model->index(i, TableIndexesModel::IndexOwner), indexInfo.owner);
        model->setData(model->index(i, TableIndexesModel::IndexName), indexInfo.name);
        model->setData(model->index(i, TableIndexesModel::IndexType), (int)indexInfo.type, Qt::EditRole);
        model->setData(model->index(i, TableIndexesModel::IndexColumns), indexInfo.columns);
        model->setData(model->index(i, TableIndexesModel::IndexCompress), indexInfo.compress, Qt::EditRole);
        if(indexInfo.prefixLength!=-1){
            model->setData(model->index(i, TableIndexesModel::IndexPrefixLength), indexInfo.prefixLength);
        }
        model->setData(model->index(i, TableIndexesModel::IndexParallel), indexInfo.parallel, Qt::EditRole);
        if(indexInfo.parallelDegree!=-1){
            model->setData(model->index(i, TableIndexesModel::IndexParallelDegree), indexInfo.parallelDegree);
        }
        model->setData(model->index(i, TableIndexesModel::IndexReverse), indexInfo.reverse, Qt::EditRole);
        saveStorageParamsToModel(model, i, indexInfo);
        model->setData(model->index(i, TableIndexesModel::IndexPartitioningType), DbUtil::getIndexPartitioningTypeName(indexInfo.partitioning, true), Qt::EditRole);
        model->setData(model->index(i, TableIndexesModel::IndexPartitioningType), indexInfo.partitioning, Qt::UserRole+1);

        if(!indexInfo.partInfo.isEmpty){
            savePartitionParamsToModel(i, indexInfo.partInfo);
        }
    }

    table->resizeColumnAccountingForEditor(TableIndexesModel::IndexOwner);
    table->resizeColumnAccountingForEditor(TableIndexesModel::IndexName);
    table->resizeColumnAccountingForEditor(TableIndexesModel::IndexColumns);

    table->setUpdatesEnabled(true);

    if(getCreatorMode() == DbObjectCreator::EditExisting){
        int lastRowIx=model->rowCount()-1;
        model->freezeRow(lastRowIx);
    }
}

void TableCreatorIndexes::saveStorageParamsToModel(TableIndexesModel *model, int rowIx, const IndexInfo &ixInfo)
{
    model->setData(model->index(rowIx, TableIndexesModel::IndexStorage), QVariant::fromValue(ixInfo.storageParams), Qt::UserRole);
    model->setData(model->index(rowIx, TableIndexesModel::IndexStorage), QVariant::fromValue(ixInfo.storageParams), Qt::UserRole+1); //original value
}

void TableCreatorIndexes::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    table->setColumnHidden(TableIndexesModel::IndexOwner, !show);
    table->setColumnHidden(TableIndexesModel::IndexCompress, !show);
    table->setColumnHidden(TableIndexesModel::IndexPrefixLength, !show);
    table->setColumnHidden(TableIndexesModel::IndexParallel, !show);
    table->setColumnHidden(TableIndexesModel::IndexParallelDegree, !show);
    table->setColumnHidden(TableIndexesModel::IndexStorage, !show);
    table->setColumnHidden(TableIndexesModel::IndexReverse, !show);
    table->setColumnHidden(TableIndexesModel::IndexPartitioningType, !show);
    table->setColumnHidden(TableIndexesModel::IndexPartitionDefinition, !show);
    table->setUpdatesEnabled(true);

}

QList<IndexInfo> TableCreatorIndexes::getIndexesInfo() const
{
    QList<IndexInfo> results;

    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    int rowCount=model->rowCount();

    IndexInfo ixInfo;

    for(int i=0; i<rowCount; ++i){
        ixInfo=model->itemInfoFromModelRow(i);
        if(ixInfo.indexId==-1){
            continue;
        }

        results.append(ixInfo);
    }

    return results;
}

QList< NameQueryPair > TableCreatorIndexes::generateAlterDdl() const
{
    Q_ASSERT(originalIndexList);

    QList< NameQueryPair > result;

    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    int rowCount=model->rowCount();

    QString schema=objectCreator->getSchemaName();
    QString tableName=objectCreator->getTableName();
    QString fullTableName=QString("\"%1\".\"%2\"").arg(schema).arg(tableName);

    IndexInfo ixInfo;
    IndexInfo originalIxInfo;

    for(int i=0; i<rowCount; ++i){
        ixInfo=model->itemInfoFromModelRow(i);
        if(ixInfo.indexId==-1){
            continue;
        }

        if(model->isRowFrozen(i)){
            originalIxInfo=originalIndexList->at(i);
            QList< NameQueryPair > alterDdls=ixInfo.generateDiffDdl(originalIxInfo, schema, tableName, objectCreator->getTableType());
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            result.append(qMakePair(QString("add_index_%1").arg(i+1), ixInfo.generateDdl(fullTableName, objectCreator->getTableType())));
        }
    }

    return result;
}

void TableCreatorIndexes::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalIndexList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("add_")){
        Q_ASSERT(originalIndexList->size()>rowIx);
    }

    const IndexInfo modifiedIxInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("add_index_")){

        originalIndexList->append(modifiedIxInfo);
        model->freezeRow(rowIx);
        saveStorageParamsToModel(model, rowIx, modifiedIxInfo);
        savePartitionParamsToModel(rowIx, modifiedIxInfo.partInfo);

    }else if(taskName.startsWith("rename_index_")){

        (*originalIndexList)[rowIx].name=modifiedIxInfo.name;

    }else if(taskName.startsWith("drop_index_for_recreation_") ||
             taskName.startsWith("drop_index_")){

        (*originalIndexList)[rowIx].dropped=true;

    }else if(taskName.startsWith("create_index_after_dropping_")){

        (*originalIndexList)[rowIx]=modifiedIxInfo;
        saveStorageParamsToModel(model, rowIx, modifiedIxInfo);

    }else if(taskName.startsWith("alter_index_")){

        TablePartitioningInfo partInfo=(*originalIndexList)[rowIx].partInfo;
        (*originalIndexList)[rowIx]=modifiedIxInfo;
        (*originalIndexList)[rowIx].partInfo=partInfo;

        saveStorageParamsToModel(model, rowIx, modifiedIxInfo);

    }else if(taskName.startsWith("table_partition")){

        //burda bug var. deyishiklikler duzgun commit olunmurlar
        QString modifiedTaskName=taskName.mid(0, taskName.lastIndexOf('_'));
        (*originalIndexList)[rowIx].partInfo.partitionInfo.commitChanges(modifiedTaskName, modifiedIxInfo.partInfo.partitionInfo);

        savePartitionParamsToModel(rowIx, (*originalIndexList)[rowIx].partInfo, modifiedTaskName);

    }

    ItemCreatorHelper::markDataChanges<IndexInfo,TableIndexesModel>(model, rowIx,
                                                                    originalIndexList);
}

void TableCreatorIndexes::alterQueryError(const QString &taskName)
{
    if(taskName.startsWith("create_index_after_dropping_")){
        Q_ASSERT(originalIndexList);
        ItemCreatorHelper::moveRowAfterCreationError(table, taskName, originalIndexList);
    }
}

void TableCreatorIndexes::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalIndexList, from, to);
}

void TableCreatorIndexes::removeIncorrectRows()
{
    TableCreatorTabWithTableView::removeIncorrectRows();

    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    int rowCount=model->rowCount();

    IndexInfo ixInfo;
    for(int i=0; i<rowCount; ++i){
        ixInfo=model->itemInfoFromModelRow(i);
        if(!ixInfo.partInfo.isEmpty){
            ixInfo.partInfo.partitionInfo.removeIncorrectRows();
            savePartitionParamsToModel(i, ixInfo.partInfo);
        }
    }
}

void TableCreatorIndexes::currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName)
{
    TableIndexesModel *model=static_cast<TableIndexesModel*>(table->model());
    int rowCount=model->rowCount();

    QModelIndex index;
    for(int i=0; i<rowCount; ++i){
        index=model->index(i, TableIndexesModel::IndexOwner);
        if(index.data().toString().trimmed()==oldSchemaName){
            model->setData(index, newSchemaName);
        }
    }
}
