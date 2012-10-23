#include "usercreatorgrantsadvancedlayout.h"
#include "interfaces/iqueryscheduler.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/rolegrantsmodel.h"
#include "models/sysprivgrantsmodel.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"
#include "beans/userinfo.h"
#include <QtGui>

UserCreatorGrantsAdvancedLayout::UserCreatorGrantsAdvancedLayout(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    tab = new QTabWidget();
    tab->setTabPosition(QTabWidget::South);

    rolesTable = new DataTableAndToolBarWidget(0, Qt::Horizontal);
    tab->addTab(rolesTable, tr("Roles"));

    sysPrivTable = new DataTableAndToolBarWidget(0, Qt::Horizontal);
    tab->addTab(sysPrivTable, tr("System privileges"));

    quotasTable = new DataTableAndToolBarWidget(0, Qt::Horizontal);
    tab->addTab(quotasTable, tr("Quotas"));

    objectPrivTable = new DataTableAndToolBarWidget(0, Qt::Horizontal);
    tab->addTab(objectPrivTable, tr("Object privileges"));

    mainLayout->addWidget(tab);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void UserCreatorGrantsAdvancedLayout::setQueryScheduler(IQueryScheduler *queryScheduler, bool editMode)
{
    this->queryScheduler = queryScheduler;

    customizeRolesTable(editMode);
    customizeSysPrivTable(editMode);
}

void UserCreatorGrantsAdvancedLayout::setUserInfo(UserInfo *userInfo)
{
    originalRoleList=&userInfo->roles;
    originalSysPrivList=&userInfo->sysPrivs;

    if(originalRoleList->size()>0){
        populateTableWithRoles();
    }

    if(originalSysPrivList->size()>0){
        populateTableWithSysPrivs();
    }
}

DataTableAndToolBarWidget *UserCreatorGrantsAdvancedLayout::getRolesTable() const
{
    return rolesTable;
}

DataTableAndToolBarWidget *UserCreatorGrantsAdvancedLayout::getSysPrivsTable() const
{
    return sysPrivTable;
}

QList<PrivGrantInfo> UserCreatorGrantsAdvancedLayout::getUserRoles() const
{
    QList<PrivGrantInfo> results;

    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(rolesTable->table()->model());
    int rowCount=model->rowCount();

    PrivGrantInfo info;

    for(int i=0; i<rowCount; ++i){
        info=model->itemInfoFromModelRow(i);
        if(info.grantId==-1){
            continue;
        }

        results.append(info);
    }

    return results;
}

QList<PrivGrantInfo> UserCreatorGrantsAdvancedLayout::getUserSysPrivs() const
{
    QList<PrivGrantInfo> results;

    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(sysPrivTable->table()->model());
    int rowCount=model->rowCount();

    PrivGrantInfo info;

    for(int i=0; i<rowCount; ++i){
        info=model->itemInfoFromModelRow(i);
        if(info.grantId==-1){
            continue;
        }

        results.append(info);
    }

    return results;
}

void UserCreatorGrantsAdvancedLayout::customizeRolesTable(bool editMode)
{
    DataTable *table=rolesTable->table();

    QStringList columnNames;
    columnNames.append(tr("Role"));
    columnNames.append(tr("Grantable"));
    columnNames.append(tr("Default"));

    RoleGrantsModel *tableModel=new RoleGrantsModel(columnNames, this);
    rolesTable->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);
    table->setColumnWidth(RoleGrantsModel::RoleName, 250);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *roleListDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                "get_role_list", "users",
                                                                this, true);

    table->setItemDelegateForColumn(RoleGrantsModel::RoleName, roleListDelegate);

    BooleanDelegate *isGrantableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(RoleGrantsModel::IsGrantable, isGrantableDelegate);

    BooleanDelegate *isDefaultDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(RoleGrantsModel::IsDefault, isDefaultDelegate);


    if(editMode){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(rolesTableDataChanged(QModelIndex,QModelIndex)));

    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void UserCreatorGrantsAdvancedLayout::customizeSysPrivTable(bool editMode)
{
    DataTable *table=sysPrivTable->table();

    QStringList columnNames;
    columnNames.append(tr("Privilege"));
    columnNames.append(tr("Grantable"));

    SysPrivGrantsModel *tableModel=new SysPrivGrantsModel(columnNames, this);
    sysPrivTable->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(150);
    table->setColumnWidth(RoleGrantsModel::RoleName, 250);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *privListDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                "get_priv_list", "grants",
                                                                this, true);

    table->setItemDelegateForColumn(SysPrivGrantsModel::PrivName, privListDelegate);

    BooleanDelegate *isGrantableDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(SysPrivGrantsModel::IsGrantable, isGrantableDelegate);

    if(editMode){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sysPrivTableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void UserCreatorGrantsAdvancedLayout::populateTableWithRoles()
{
    Q_ASSERT(originalRoleList);

    DataTable *table=rolesTable->table();

    table->setUpdatesEnabled(false);

    int consCount=originalRoleList->count();

    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(table->model());
    model->ensureRowCount(consCount);

    PrivGrantInfo info;
    for(int i=0; i<originalRoleList->count(); ++i){
        info = originalRoleList->at(i);

        model->setData(model->index(i, RoleGrantsModel::RoleName), info.name);
        model->setData(model->index(i, RoleGrantsModel::IsGrantable), (int)info.isGrantable, Qt::EditRole);
        model->setData(model->index(i, RoleGrantsModel::IsDefault), (int)info.isDefault, Qt::EditRole);
    }

    table->resizeColumnsAccountingForEditor();

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
}

void UserCreatorGrantsAdvancedLayout::populateTableWithSysPrivs()
{
    Q_ASSERT(originalSysPrivList);

    DataTable *table=sysPrivTable->table();

    table->setUpdatesEnabled(false);

    int consCount=originalSysPrivList->count();

    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(table->model());
    model->ensureRowCount(consCount);

    PrivGrantInfo info;
    for(int i=0; i<originalSysPrivList->count(); ++i){
        info = originalSysPrivList->at(i);

        model->setData(model->index(i, SysPrivGrantsModel::PrivName), info.name);
        model->setData(model->index(i, SysPrivGrantsModel::IsGrantable), (int)info.isGrantable, Qt::EditRole);
    }

    table->resizeColumnsAccountingForEditor();

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx);
}

void UserCreatorGrantsAdvancedLayout::rolesTableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(rolesTable->table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalRoleList, from, to);
}

void UserCreatorGrantsAdvancedLayout::sysPrivTableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(sysPrivTable->table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalSysPrivList, from, to);
}

void UserCreatorGrantsAdvancedLayout::alterQuerySucceeded(const QString &taskName)
{
    if(taskName.startsWith("role")){
        roleAlterQuerySucceeded(taskName);
    }else if(taskName.startsWith("sys_priv")){
        sysPrivAlterQuerySucceeded(taskName);
    }
}

void UserCreatorGrantsAdvancedLayout::alterQueryError(const QString &taskName)
{
    if(taskName.startsWith("role")){
        roleAlterQueryError(taskName);
    }else if(taskName.startsWith("sys_priv")){
        sysPrivAlterQueryError(taskName);
    }
}

void UserCreatorGrantsAdvancedLayout::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(rolesTable->table());
    WidgetHelper::removeIncorrectRows(sysPrivTable->table());
}

void UserCreatorGrantsAdvancedLayout::roleAlterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalRoleList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    RoleGrantsModel *model=static_cast<RoleGrantsModel*>(rolesTable->table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("role_add_")){
        Q_ASSERT(originalRoleList->size()>rowIx);
    }

    PrivGrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("role_add_priv_grant_")){

        originalRoleList->append(modifiedGrantInfo);
        model->freezeRow(rowIx);

    }else if(taskName.startsWith("role_drop_priv_grant__")){

        (*originalRoleList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_drop_priv_grant_for_recreation_")){

        (*originalRoleList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_create_priv_grant_after_dropping_")){

        (*originalRoleList)[rowIx]=modifiedGrantInfo;

    }else if(taskName=="role_set_default"){

        QList<PrivGrantInfo> roles=getUserRoles();
        Q_ASSERT(originalRoleList->size()>=roles.size());
        for(int i=0; i<roles.size(); ++i){
            const PrivGrantInfo &role=roles.at(i);
            if(originalRoleList->at(i).isDefault!=role.isDefault){
                ((*originalRoleList)[i]).isDefault=role.isDefault;
            }
        }

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalRoleList);
}

void UserCreatorGrantsAdvancedLayout::roleAlterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("role_create_priv_grant_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(rolesTable->table(), taskName, originalRoleList);
}

void UserCreatorGrantsAdvancedLayout::sysPrivAlterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalSysPrivList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    SysPrivGrantsModel *model=static_cast<SysPrivGrantsModel*>(sysPrivTable->table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("sys_priv_add_")){
        Q_ASSERT(originalSysPrivList->size()>rowIx);
    }

    PrivGrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("sys_priv_add_priv_grant_")){

        originalSysPrivList->append(modifiedGrantInfo);
        model->freezeRow(rowIx);

    }else if(taskName.startsWith("sys_priv_drop_priv_grant__")){

        (*originalSysPrivList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_drop_priv_grant_for_recreation_")){

        (*originalSysPrivList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_create_priv_grant_after_dropping_")){

        (*originalSysPrivList)[rowIx]=modifiedGrantInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalSysPrivList);
}

void UserCreatorGrantsAdvancedLayout::sysPrivAlterQueryError(const QString &taskName)
{
    if(!taskName.startsWith("sys_priv_create_priv_grant_after_dropping_")){
        return;
    }

    ItemCreatorHelper::moveRowAfterCreationError(sysPrivTable->table(), taskName, originalSysPrivList);
}
