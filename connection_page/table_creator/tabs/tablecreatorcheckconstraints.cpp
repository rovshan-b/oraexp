#include "tablecreatorcheckconstraints.h"
#include "connectivity/dbconnection.h"
#include "delegates/identifiernamedelegate.h"
#include "delegates/booleandelegate.h"
#include "delegates/plaintexteditordelegate.h"
#include "../tablecreatortabs.h"
#include "util/itemcreatorhelper.h"
#include <QtGui>

TableCreatorCheckConstraints::TableCreatorCheckConstraints(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    TableCreatorTabWithTableView(tableCreator, editMode, parent), originalConstraintList(0)
{

}

void TableCreatorCheckConstraints::setConnection(DbConnection *db)
{
    TableCreatorTabWithTableView::setConnection(db);

    customizeTableWidget();

    //if(isEditMode()){
    //    loadConstraints();
    //}
}

void TableCreatorCheckConstraints::customizeTableWidget()
{
    QStringList columnNames;
    columnNames.append(tr("Name"));
    columnNames.append(tr("Condition"));
    columnNames.append(tr("Deferrable"));
    columnNames.append(tr("Deferred"));

    TableCheckConstraintsModel *tableModel=new TableCheckConstraintsModel(columnNames, this);
    table->setModel(tableModel);

    table->setColumnWidth(0, 150);
    table->setColumnWidth(1, 350);
    table->setColumnWidth(2, 80);
    table->setColumnWidth(3, 80);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    IdentifierNameDelegate *constraintNameDelegate=new IdentifierNameDelegate(db, this);
    constraintNameDelegate->setConnection(db);
    table->setItemDelegateForColumn(TableCheckConstraintsModel::ConstraintName, constraintNameDelegate);

    PlainTextEditorDelegate *conditionDelegate=new PlainTextEditorDelegate(tr("Edit constraint condition"), this);
    table->setItemDelegateForColumn(TableCheckConstraintsModel::ConstraintCondition, conditionDelegate);

    BooleanDelegate *deferrableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableCheckConstraintsModel::ConstraintDeferrable, deferrableDelegate);

    BooleanDelegate *deferredDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TableCheckConstraintsModel::ConstraintDeferred, deferredDelegate);

    //table->verticalHeader()->setVisible(false);
    table->model()->insertRows(0, 1);

    showAdvancedOptions(false);

    if(isEditMode()){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}


void TableCreatorCheckConstraints::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    table->setColumnHidden(TableCheckConstraintsModel::ConstraintDeferrable, !show);
    table->setColumnHidden(TableCheckConstraintsModel::ConstraintDeferred, !show);
    table->setUpdatesEnabled(true);
}

void TableCreatorCheckConstraints::setTableInfo(TableInfo *tableInfo)
{
    originalConstraintList=&tableInfo->checkConstraints;
    if(originalConstraintList->size()>0){
        populateTableWithConstraints();
    }
}

void TableCreatorCheckConstraints::populateTableWithConstraints()
{
    Q_ASSERT(originalConstraintList);

    table->setUpdatesEnabled(false);

    int consCount=originalConstraintList->count();

    TableCheckConstraintsModel *model=static_cast<TableCheckConstraintsModel*>(table->model());
    model->ensureRowCount(consCount);

    ConstraintInfo consInfo;
    for(int i=0; i<originalConstraintList->count(); ++i){
        consInfo = originalConstraintList->at(i);

        model->setData(model->index(i, TableCheckConstraintsModel::ConstraintName), consInfo.name);
        model->setData(model->index(i, TableCheckConstraintsModel::ConstraintCondition), consInfo.searchCondition);
        model->setData(model->index(i, TableCheckConstraintsModel::ConstraintDeferrable), consInfo.deferrable, Qt::EditRole);
        model->setData(model->index(i, TableCheckConstraintsModel::ConstraintDeferred), consInfo.deferred, Qt::EditRole);
    }

    table->resizeColumnAccountingForEditor(TableCheckConstraintsModel::ConstraintName);
    table->resizeColumnAccountingForEditor(TableCheckConstraintsModel::ConstraintCondition);

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
}


QList<ConstraintInfo> TableCreatorCheckConstraints::getConstraintsInfo() const
{
    QList<ConstraintInfo> results;

    TableCheckConstraintsModel *model=static_cast<TableCheckConstraintsModel*>(table->model());
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

void TableCreatorCheckConstraints::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalConstraintList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TableCheckConstraintsModel *model=static_cast<TableCheckConstraintsModel*>(table->model());
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

void TableCreatorCheckConstraints::alterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("create_table_constraint_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(table, taskName, originalConstraintList);
}

void TableCreatorCheckConstraints::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TableCheckConstraintsModel *model=static_cast<TableCheckConstraintsModel*>(table->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalConstraintList, from, to);
}

