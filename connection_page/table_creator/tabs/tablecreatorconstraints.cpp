#include "tablecreatorconstraints.h"
#include "../tablecreatortabs.h"
#include "tablecreatorcolumns.h"
#include "tablecreatorgeneralinfo.h"
#include "interfaces/istringlistretriever.h"
#include "delegates/schemaselectordelegate.h"
#include "delegates/tableselectordelegate.h"
#include "delegates/booleandelegate.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/constraintselectordelegate.h"
#include "delegates/columnselectordelegate.h"
#include "delegates/identifiernamedelegate.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/itemcreatorhelper.h"
#include "util/dbutil.h"
#include <QtGui>

#include <memory>
using namespace std;

TableCreatorConstraints::TableCreatorConstraints(const QString &schemaName,
                                                 TableCreatorTabs *tableCreator,
                                                 bool editMode,
                                                 QWidget *parent) :
    TableCreatorTabWithTableView(tableCreator, editMode, parent),
    schemaName(schemaName),
    originalConstraintList(0)
{

}

void TableCreatorConstraints::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    TableCreatorTabWithTableView::setQueryScheduler(queryScheduler);

    customizeTableWidget(schemaName);

    //if(isEditMode()){
    //    loadConstraints();
    //}
}


void TableCreatorConstraints::customizeTableWidget(const QString &schemaName)
{
    QStringList columnNames;
    columnNames.append(tr("Name"));
    columnNames.append(tr("Type"));
    columnNames.append(tr("Columns"));
    columnNames.append(tr("FK schema"));
    columnNames.append(tr("FK table"));
    columnNames.append(tr("FK columns"));
    columnNames.append(tr("On Delete"));
    columnNames.append(tr("Deferrable"));
    columnNames.append(tr("Deferred"));

    table->horizontalHeader()->setDefaultSectionSize(150);

    TableConstraintsModel *tableModel=new TableConstraintsModel(columnNames, this);
    table->setModel(tableModel);

    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    IdentifierNameDelegate *constraintNameDelegate=new IdentifierNameDelegate(this);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintName, constraintNameDelegate);

    IndexBasedComboBoxDelegate *constraintTypeDelegate=new IndexBasedComboBoxDelegate(this, TableConstraintsModel::ConstraintConstraintType);
    tableModel->setList(TableConstraintsModel::ConstraintConstraintType, DbUtil::getConstraintTypeNames());
    tableModel->setIconList(TableConstraintsModel::ConstraintConstraintType, DbUtil::getConstraintTypeIcons());
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintConstraintType, constraintTypeDelegate);

    ColumnSelectorDelegate *columnsDelegate=new ColumnSelectorDelegate(objectCreator->getColumnsTab(), tr("Select columns"), this);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintColumns, columnsDelegate);

    SchemaSelectorDelegate *schemaListDelegate=new SchemaSelectorDelegate(schemaName, this->queryScheduler, this);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintReferencedSchema, schemaListDelegate);

    TableSelectorDelegate *tableListDelegate=new TableSelectorDelegate(schemaName, TableConstraintsModel::ConstraintReferencedSchema, this->queryScheduler, this);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintReferencedTable, tableListDelegate);

    ConstraintSelectorDelegate *referencedColumnsDelegate=new ConstraintSelectorDelegate(schemaName,
                                                                                 TableConstraintsModel::ConstraintReferencedSchema,
                                                                                 TableConstraintsModel::ConstraintReferencedTable,
                                                                                 OraExp::PrimaryKey,
                                                                                 "", this->queryScheduler, this);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintReferencedColumn, referencedColumnsDelegate);

    IndexBasedComboBoxDelegate *onDeleteDelegate=new IndexBasedComboBoxDelegate(this, TableConstraintsModel::ConstraintOnDeleteAction);
    tableModel->setList(TableConstraintsModel::ConstraintOnDeleteAction, DbUtil::getOnDeleteActionNames());
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintOnDeleteAction, onDeleteDelegate);
    table->setColumnWidth(TableConstraintsModel::ConstraintOnDeleteAction, 80);

    BooleanDelegate *deferrableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintDeferrable, deferrableDelegate);
    table->setColumnWidth(TableConstraintsModel::ConstraintDeferrable, 80);

    BooleanDelegate *deferredDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableConstraintsModel::ConstraintDeferred, deferredDelegate);
    table->setColumnWidth(TableConstraintsModel::ConstraintDeferred, 80);


    //table->verticalHeader()->setVisible(false);
    table->model()->insertRows(0, 1);

    showAdvancedOptions(false);

    if(isEditMode()){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void TableCreatorConstraints::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    table->setColumnHidden(TableConstraintsModel::ConstraintReferencedSchema, !show);
    table->setColumnHidden(TableConstraintsModel::ConstraintDeferrable, !show);
    table->setColumnHidden(TableConstraintsModel::ConstraintDeferred, !show);
    table->setUpdatesEnabled(true);
}

void TableCreatorConstraints::setTableType(OraExp::TableType tableType)
{
    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    model->setTableType(tableType);
}

void TableCreatorConstraints::setTableInfo(TableInfo *tableInfo)
{
    originalConstraintList=&tableInfo->constraints;
    if(originalConstraintList->size()>0){
        populateTableWithConstraints();
    }
}

QStringList TableCreatorConstraints::getStringList(const QString &type)
{
    QStringList items;

    if(type=="fk"){
        TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
        int rowCount=model->rowCount();
        QString consName;
        QVariant consType;

        for(int i=0; i<rowCount; ++i){
            consType=model->index(i, TableConstraintsModel::ConstraintConstraintType).data(Qt::EditRole);
            if(!consType.isValid()){
                continue;
            }
            OraExp::ConstraintType type=(OraExp::ConstraintType)consType.toInt();

            if(type!=OraExp::ForeignKey){
                continue;
            }

            if(model->index(i, TableConstraintsModel::ConstraintReferencedSchema).data().toString().trimmed().isEmpty() ||
                model->index(i, TableConstraintsModel::ConstraintReferencedTable).data().toString().trimmed().isEmpty() ||
                model->index(i, TableConstraintsModel::ConstraintReferencedColumn).data().toString().trimmed().isEmpty()){
                    continue;
            }

            consName=model->index(i, TableConstraintsModel::ConstraintName).data().toString().trimmed();
            if(!consName.isEmpty()){
                items.append(consName);
            }

        }
    }

    return items;
}

void TableCreatorConstraints::populateTableWithConstraints()
{
    Q_ASSERT(originalConstraintList);

    table->setUpdatesEnabled(false);

    int consCount=originalConstraintList->count();

    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    model->ensureRowCount(consCount);

    ConstraintInfo consInfo;
    for(int i=0; i<originalConstraintList->count(); ++i){
        consInfo = originalConstraintList->at(i);

        model->setData(model->index(i, TableConstraintsModel::ConstraintName), consInfo.name);
        model->setData(model->index(i, TableConstraintsModel::ConstraintConstraintType), (int)consInfo.type, Qt::EditRole);
        model->setData(model->index(i, TableConstraintsModel::ConstraintColumns), consInfo.columns);
        if(consInfo.type==OraExp::ForeignKey){
            model->setData(model->index(i, TableConstraintsModel::ConstraintReferencedSchema), consInfo.rOwner);
            model->setData(model->index(i, TableConstraintsModel::ConstraintReferencedTable), consInfo.rTableName);
            model->setData(model->index(i, TableConstraintsModel::ConstraintReferencedColumn), consInfo.rColumns);
            model->setData(model->index(i, TableConstraintsModel::ConstraintOnDeleteAction), consInfo.deleteRule, Qt::EditRole);
        }
        model->setData(model->index(i, TableConstraintsModel::ConstraintDeferrable), consInfo.deferrable, Qt::EditRole);
        model->setData(model->index(i, TableConstraintsModel::ConstraintDeferred), consInfo.deferred, Qt::EditRole);
    }

    table->resizeColumnAccountingForEditor(TableConstraintsModel::ConstraintName);
    table->resizeColumnAccountingForEditor(TableConstraintsModel::ConstraintColumns);
    table->resizeColumnAccountingForEditor(TableConstraintsModel::ConstraintReferencedSchema);
    table->resizeColumnAccountingForEditor(TableConstraintsModel::ConstraintReferencedTable);

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
}

QList<ConstraintInfo> TableCreatorConstraints::getConstraintsInfo() const
{
    QList<ConstraintInfo> results;

    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    int rowCount=model->rowCount();

    ConstraintInfo consInfo;

    for(int i=0; i<rowCount; ++i){
        consInfo=model->itemInfoFromModelRow(i);
        if(consInfo.constraintId==-1){
            continue;
        }

        results.append(consInfo);
    }

    return results;
}

void TableCreatorConstraints::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalConstraintList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("add_")){
        Q_ASSERT(originalConstraintList->size()>rowIx);
    }

    ConstraintInfo modifiedConsInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("add_table_constraint_")){

        originalConstraintList->append(modifiedConsInfo);
        model->freezeRow(rowIx);

    }else if(taskName.startsWith("drop_table_constraint_")){

        (*originalConstraintList)[rowIx].dropped=true;

    }else if(taskName.startsWith("rename_table_constraint_")){

        (*originalConstraintList)[rowIx].name=modifiedConsInfo.name;

    }else if(taskName.startsWith("drop_table_constraint_for_recreation_")){

        (*originalConstraintList)[rowIx].dropped=true;

    }else if(taskName.startsWith("create_table_constraint_after_dropping_")){

        (*originalConstraintList)[rowIx]=modifiedConsInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalConstraintList);
}

void TableCreatorConstraints::alterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("create_table_constraint_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(table, taskName, originalConstraintList);
}

void TableCreatorConstraints::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalConstraintList, from, to);
}

void TableCreatorConstraints::currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName)
{
    TableConstraintsModel *model=static_cast<TableConstraintsModel*>(table->model());
    int rowCount=model->rowCount();

    QModelIndex index;
    for(int i=0; i<rowCount; ++i){
        index=model->index(i, TableConstraintsModel::ConstraintReferencedSchema);
        if(index.data().toString().trimmed()==oldSchemaName){
            model->setData(index, newSchemaName);
        }
    }
}
