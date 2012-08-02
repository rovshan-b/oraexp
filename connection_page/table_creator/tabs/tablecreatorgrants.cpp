#include "tablecreatorgrants.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "delegates/schemaselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/itemcreatorhelper.h"
#include "../tablecreatortabs.h"
#include <QtGui>

TableCreatorGrants::TableCreatorGrants(const QString &schemaName, TableCreatorTabs *tableCreator, bool editMode, QWidget *parent) :
    TableCreatorTabWithTableView(tableCreator, editMode, parent), schemaName(schemaName), originalGrantList(0)
{

}

void TableCreatorGrants::setConnection(DbConnection *db)
{
    TableCreatorTabWithTableView::setConnection(db);

    customizeTableWidget(schemaName);

    //if(isEditMode()){
    //    loadGrants();
    //}
}

void TableCreatorGrants::customizeTableWidget(const QString &schemaName)
{
    QStringList columnNames;
    columnNames.append(tr("User"));
    DbUtil::populatePrivilegeNames(columnNames);

    TableGrantsModel *tableModel=new TableGrantsModel(columnNames, this);
    table->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    schemaListDelegate=new SchemaSelectorDelegate(schemaName, tableCreator->scheduler(), this, true);
    table->setItemDelegateForColumn(TableGrantsModel::GrantSchema, schemaListDelegate);

    QStringList grantTypes;
    grantTypes.append("");
    grantTypes.append(tr("GRANT"));
    grantTypes.append(tr("WITH GRANT OPTION"));

    QIcon grantIcon=QIcon();

    IndexBasedComboBoxDelegate *selectDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantSelect, selectDelegate);

    IndexBasedComboBoxDelegate *insertDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantInsert, insertDelegate);

    IndexBasedComboBoxDelegate *updateDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantUpdate, updateDelegate);

    IndexBasedComboBoxDelegate *deleteDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantDelete, deleteDelegate);

    IndexBasedComboBoxDelegate *alterDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantAlter, alterDelegate);

    IndexBasedComboBoxDelegate *indexDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantIndex, indexDelegate);

    IndexBasedComboBoxDelegate *referencesDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantReferences, referencesDelegate);

    IndexBasedComboBoxDelegate *debugDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
    table->setItemDelegateForColumn(TableGrantsModel::GrantDebug, debugDelegate);

    //table->verticalHeader()->setVisible(false);
    table->model()->insertRows(0, 1);

    showAdvancedOptions(false);

    if(isEditMode()){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void TableCreatorGrants::setTableInfo(TableInfo *tableInfo)
{
    originalGrantList=&tableInfo->grants;
    if(originalGrantList->size()>0){
        populateTableWithGrants();
    }
}

void TableCreatorGrants::populateTableWithGrants()
{
    Q_ASSERT(originalGrantList);

    table->setUpdatesEnabled(false);

    int colCount=originalGrantList->count();

    TableGrantsModel *model=static_cast<TableGrantsModel*>(table->model());
    model->ensureRowCount(colCount);

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames);

    GrantInfo grantInfo;
    for(int i=0; i<originalGrantList->count(); ++i){
        grantInfo = originalGrantList->at(i);

        model->setData(model->index(i, TableGrantsModel::GrantSchema), grantInfo.grantee);

        for(int k=0; k<privilegeNames.size(); ++k){
            model->setData(model->index(i, k+1 /*1st column is schema name*/), grantInfo.privileges.value(privilegeNames.at(k), 0), Qt::EditRole);
        }
    }

    table->resizeColumnAccountingForEditor(TableGrantsModel::GrantSchema);

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
    model->setColumnEnabled(TableGrantsModel::GrantSchema, false, lastRowIx);
}

void TableCreatorGrants::showAdvancedOptions(bool show)
{
    table->setUpdatesEnabled(false);
    table->setColumnHidden(TableGrantsModel::GrantAlter, !show);
    table->setColumnHidden(TableGrantsModel::GrantIndex, !show);
    table->setColumnHidden(TableGrantsModel::GrantReferences, !show);
    table->setColumnHidden(TableGrantsModel::GrantDebug, !show);
    table->setUpdatesEnabled(true);

}

QList<GrantInfo> TableCreatorGrants::getGrantsInfo() const
{
    QList<GrantInfo> results;

    TableGrantsModel *model=static_cast<TableGrantsModel*>(table->model());
    int rowCount=model->rowCount();

    GrantInfo grantInfo;
    for(int i=0; i<rowCount; ++i){
        grantInfo=model->itemInfoFromModelRow(i);
        if(grantInfo.grantId==-1){
            continue;
        }

        results.append(grantInfo);
    }

    return results;
}

void TableCreatorGrants::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalGrantList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TableGrantsModel *model=static_cast<TableGrantsModel*>(table->model());
    int rowIx=rowNum-1;

    //if(!taskName.startsWith("add_")){
    //    Q_ASSERT(originalGrantList->size()>rowIx);
    //}

    GrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("add_table_grant_")){

        if(rowIx>=originalGrantList->size()){
            originalGrantList->append(modifiedGrantInfo);
            model->freezeRow(rowIx);
            model->setColumnEnabled(TableGrantsModel::GrantSchema, false, rowIx);
        }

        int lastUnderscoreIx=taskName.lastIndexOf('_');
        int prevUnderscoreIx=taskName.lastIndexOf('_', lastUnderscoreIx-1);
        QString privilegeName=taskName.mid(prevUnderscoreIx+1, lastUnderscoreIx-prevUnderscoreIx-1);
        qDebug() << privilegeName;

        QStringList privilegeNames;
        DbUtil::populatePrivilegeNames(privilegeNames);
        int colIx=privilegeNames.indexOf(privilegeName)+1;
        Q_ASSERT(colIx>0);

        (*originalGrantList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("revoke_all_privileges_")){

        (*originalGrantList)[rowIx].dropped=true;

    }else if(taskName.startsWith("change_grant_type_")){

        (*originalGrantList)[rowIx]=modifiedGrantInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalGrantList);
}

void TableCreatorGrants::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TableGrantsModel *model=static_cast<TableGrantsModel*>(table->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalGrantList, from, to);
}

void TableCreatorGrants::currentSchemaChanged(const QString &/*oldSchemaName*/, const QString &newSchemaName)
{
    schemaListDelegate->setDefaultSchemaName(newSchemaName);
}
