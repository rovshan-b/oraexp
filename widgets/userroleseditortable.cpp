#include "userroleseditortable.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"
#include "models/rolegrantsmodel.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"

UserRolesEditorTable::UserRolesEditorTable(DbObjectCreator::CreatorMode creatorMode, QWidget *parent) :
    DataTableAndToolBarWidget(0, Qt::Horizontal, parent),
    queryScheduler(0),
    creatorMode(creatorMode),
    originalItemList(0)
{
}

void UserRolesEditorTable::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    customizeTable();
}

QList<PrivGrantInfo> UserRolesEditorTable::getList() const
{
    return ItemCreatorHelper::getItemList<PrivGrantInfo,RoleGrantsModel>(
                static_cast<RoleGrantsModel*>(table()->model()));
}

void UserRolesEditorTable::populateTable(QList<PrivGrantInfo> *originalItemList)
{
    Q_ASSERT(this->originalItemList==0);

    this->originalItemList=originalItemList;
    if(this->originalItemList->size()>0){
        ItemCreatorHelper::populateTableWithItems<PrivGrantInfo,RoleGrantsModel>(originalItemList,
                                                                                 this->table(),
                                                                                 &UserRolesEditorTable::setRowData);
    }
}

void UserRolesEditorTable::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table());
}

void UserRolesEditorTable::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalItemList);

    if(taskName=="role_set_default"){
        QList<PrivGrantInfo> roles=getList();
        Q_ASSERT(originalItemList->size()>=roles.size());
        for(int i=0; i<roles.size(); ++i){
            const PrivGrantInfo &role=roles.at(i);
            if(originalItemList->at(i).isDefault!=role.isDefault){
                ((*originalItemList)[i]).isDefault=role.isDefault;
            }
        }
        return;
    }

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("role_add_")){
        Q_ASSERT(originalItemList->size()>rowIx);
    }

    PrivGrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("role_add_priv_grant_")){

        originalItemList->append(modifiedGrantInfo);
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("role_drop_priv_grant_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_drop_priv_grant_for_recreation_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_create_priv_grant_after_dropping_")){

        (*originalItemList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("role_make_grantable_")){

        (*originalItemList)[rowIx].isGrantable=modifiedGrantInfo.isGrantable;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalItemList);
}

void UserRolesEditorTable::alterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("role_create_priv_grant_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(table(), taskName, originalItemList);
}

void UserRolesEditorTable::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalItemList, from, to);
}

void UserRolesEditorTable::customizeTable()
{
    DataTable *tbl=table();

    QStringList columnNames;
    columnNames.append(tr("Role"));
    columnNames.append(tr("Grantable"));
    columnNames.append(tr("Default"));

    RoleGrantsModel *tableModel=new RoleGrantsModel(columnNames, this);
    setModel(tableModel);

    tbl->horizontalHeader()->setDefaultSectionSize(150);
    tbl->setColumnWidth(RoleGrantsModel::RoleName, 250);
    tbl->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *roleListDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                "get_role_list", "users",
                                                                this, true);

    tbl->setItemDelegateForColumn(RoleGrantsModel::RoleName, roleListDelegate);

    BooleanDelegate *isGrantableDelegate=new BooleanDelegate(this, false);
    tbl->setItemDelegateForColumn(RoleGrantsModel::IsGrantable, isGrantableDelegate);

    BooleanDelegate *isDefaultDelegate=new BooleanDelegate(this, false);
    tbl->setItemDelegateForColumn(RoleGrantsModel::IsDefault, isDefaultDelegate);

    if(creatorMode == DbObjectCreator::EditExisting){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));

        tableModel->setColumnEnabled(0, false);

    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void UserRolesEditorTable::setRowData(int rowIx, RoleGrantsModel *model, PrivGrantInfo *info)
{
    model->setData(model->index(rowIx, RoleGrantsModel::RoleName), info->name);
    model->setData(model->index(rowIx, RoleGrantsModel::IsGrantable), (int)info->isGrantable, Qt::EditRole);
    model->setData(model->index(rowIx, RoleGrantsModel::IsDefault), (int)info->isDefault, Qt::EditRole);
}
