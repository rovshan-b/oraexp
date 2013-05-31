#include "usersysprivseditortable.h"
#include "models/sysprivgrantsmodel.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"

UserSysPrivsEditorTable::UserSysPrivsEditorTable(DbObjectCreator::CreatorMode creatorMode, QWidget *parent) :
    DataTableAndToolBarWidget(0, Qt::Horizontal, parent),
    queryScheduler(0),
    creatorMode(creatorMode),
    originalItemList(0)
{
}

void UserSysPrivsEditorTable::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    customizeTable();
}

QList<PrivGrantInfo> UserSysPrivsEditorTable::getList() const
{
    return ItemCreatorHelper::getItemList<PrivGrantInfo,SysPrivGrantsModel>(
                static_cast<SysPrivGrantsModel*>(table()->model()));
}

void UserSysPrivsEditorTable::populateTable(QList<PrivGrantInfo> *originalItemList)
{
    Q_ASSERT(this->originalItemList==0);

    this->originalItemList=originalItemList;
    if(this->originalItemList->size()>0){
        ItemCreatorHelper::populateTableWithItems<PrivGrantInfo,SysPrivGrantsModel>(originalItemList,
                                                                             this->table(),
                                                                             &UserSysPrivsEditorTable::setRowData);
    }
}

void UserSysPrivsEditorTable::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table());
}

void UserSysPrivsEditorTable::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalItemList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("sys_priv_add_")){
        Q_ASSERT(originalItemList->size()>rowIx);
    }

    PrivGrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("sys_priv_add_priv_grant_")){

        originalItemList->append(modifiedGrantInfo);
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("sys_priv_drop_priv_grant_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_drop_priv_grant_for_recreation_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_create_priv_grant_after_dropping_")){

        (*originalItemList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("sys_priv_make_grantable_")){

        (*originalItemList)[rowIx].isGrantable=modifiedGrantInfo.isGrantable;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalItemList);
}

void UserSysPrivsEditorTable::alterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("sys_priv_create_priv_grant_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(table(), taskName, originalItemList);
}

void UserSysPrivsEditorTable::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalItemList, from, to);
}

void UserSysPrivsEditorTable::customizeTable()
{
    DataTable *table=this->table();

    QStringList columnNames;
    columnNames.append(tr("Privilege"));
    columnNames.append(tr("Grantable"));

    SysPrivGrantsModel *tableModel=new SysPrivGrantsModel(columnNames, this);
    setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);
    table->setColumnWidth(SysPrivGrantsModel::PrivName, 250);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *privListDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                "get_priv_list", "grants",
                                                                this, true);

    table->setItemDelegateForColumn(SysPrivGrantsModel::PrivName, privListDelegate);

    BooleanDelegate *isGrantableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(SysPrivGrantsModel::IsGrantable, isGrantableDelegate);

    if(creatorMode == DbObjectCreator::EditExisting){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));

        tableModel->setColumnEnabled(0, false);
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void UserSysPrivsEditorTable::setRowData(int rowIx, SysPrivGrantsModel *model, PrivGrantInfo *info)
{
    model->setData(model->index(rowIx, SysPrivGrantsModel::PrivName), info->name);
    model->setData(model->index(rowIx, SysPrivGrantsModel::IsGrantable), (int)info->isGrantable, Qt::EditRole);
}
