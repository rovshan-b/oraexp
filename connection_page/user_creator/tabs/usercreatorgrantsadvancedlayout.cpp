#include "usercreatorgrantsadvancedlayout.h"
#include "interfaces/iqueryscheduler.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/rolegrantsmodel.h"
#include "models/sysprivgrantsmodel.h"
#include "models/tablespacequotasmodel.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"
#include "delegates/extentsizeeditordelegate.h"
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
    customizeQuotasTable(editMode);
}

void UserCreatorGrantsAdvancedLayout::setUserInfo(UserInfo *userInfo)
{
    originalRoleList=&userInfo->roles;
    originalSysPrivList=&userInfo->sysPrivs;
    originalQuotaList=&userInfo->quotas;

    if(originalRoleList->size()>0){
        populateTableWithRoles();
    }

    if(originalSysPrivList->size()>0){
        populateTableWithSysPrivs();
    }

    if(originalQuotaList->size()>0){
        populateTableWithQuotas();
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

QList<TablespaceQuotaInfo> UserCreatorGrantsAdvancedLayout::getUserQuotas() const
{
    QList<TablespaceQuotaInfo> results;

    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(quotasTable->table()->model());
    int rowCount=model->rowCount();

    TablespaceQuotaInfo info;

    for(int i=0; i<rowCount; ++i){
        info=model->itemInfoFromModelRow(i);
        if(info.quotaId==-1){
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

        tableModel->setColumnEnabled(0, false);

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

        tableModel->setColumnEnabled(0, false);
    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void UserCreatorGrantsAdvancedLayout::customizeQuotasTable(bool editMode)
{
    DataTable *table=quotasTable->table();

    QStringList columnNames;
    columnNames.append(tr("Tablespace"));
    columnNames.append(tr("Quota"));

    TablespaceQuotasModel *tableModel=new TablespaceQuotasModel(columnNames, this);
    quotasTable->setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(250);
    //table->setColumnWidth(TablespaceQuotasModel::TablespaceName, 250);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *tablespaceNameDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                      "get_tablespace_list", "tablespace",
                                                                      this, true);
    table->setItemDelegateForColumn(TablespaceQuotasModel::TablespaceName, tablespaceNameDelegate);

    ExtentSizeEditorDelegate *quotaDelegate=new ExtentSizeEditorDelegate(this);
    table->setItemDelegateForColumn(TablespaceQuotasModel::Quota, quotaDelegate);

    if(editMode){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(quotasTableDataChanged(QModelIndex,QModelIndex)));

        tableModel->setColumnEnabled(0, false);

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
    model->freezeRow(lastRowIx, true);
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
    model->freezeRow(lastRowIx, true);
}

void UserCreatorGrantsAdvancedLayout::populateTableWithQuotas()
{
    Q_ASSERT(originalQuotaList);

    DataTable *table=quotasTable->table();

    table->setUpdatesEnabled(false);

    int consCount=originalQuotaList->count();

    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(table->model());
    model->ensureRowCount(consCount);

    TablespaceQuotaInfo info;
    for(int i=0; i<originalQuotaList->count(); ++i){
        info = originalQuotaList->at(i);

        model->setData(model->index(i, TablespaceQuotasModel::TablespaceName), info.tablespaceName);
        model->setData(model->index(i, TablespaceQuotasModel::Quota), info.toString());
    }

    table->resizeColumnsAccountingForEditor();

    table->setUpdatesEnabled(true);

    int lastRowIx=model->rowCount()-1;
    model->freezeRow(lastRowIx, true);
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

void UserCreatorGrantsAdvancedLayout::quotasTableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(quotasTable->table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalQuotaList, from, to);
}

void UserCreatorGrantsAdvancedLayout::alterQuerySucceeded(const QString &taskName)
{
    if(taskName.startsWith("role")){
        roleAlterQuerySucceeded(taskName);
    }else if(taskName.startsWith("sys_priv")){
        sysPrivAlterQuerySucceeded(taskName);
    }else if(taskName.startsWith("quota")){
        quotaAlterQuerySucceeded(taskName);
    }
}

void UserCreatorGrantsAdvancedLayout::alterQueryError(const QString &taskName)
{
    if(taskName.startsWith("role")){
        roleAlterQueryError(taskName);
    }else if(taskName.startsWith("sys_priv")){
        sysPrivAlterQueryError(taskName);
    }else if(taskName.startsWith("quota")){
        quotaAlterQueryError(taskName);
    }
}

void UserCreatorGrantsAdvancedLayout::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(rolesTable->table());
    WidgetHelper::removeIncorrectRows(sysPrivTable->table());
    WidgetHelper::removeIncorrectRows(quotasTable->table());
}

void UserCreatorGrantsAdvancedLayout::roleAlterQuerySucceeded(const QString &taskName)
{   
    Q_ASSERT(originalRoleList);

    if(taskName=="role_set_default"){
        QList<PrivGrantInfo> roles=getUserRoles();
        Q_ASSERT(originalRoleList->size()>=roles.size());
        for(int i=0; i<roles.size(); ++i){
            const PrivGrantInfo &role=roles.at(i);
            if(originalRoleList->at(i).isDefault!=role.isDefault){
                ((*originalRoleList)[i]).isDefault=role.isDefault;
            }
        }
        return;
    }

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
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("role_drop_priv_grant_")){

        (*originalRoleList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_drop_priv_grant_for_recreation_")){

        (*originalRoleList)[rowIx].dropped=true;

    }else if(taskName.startsWith("role_create_priv_grant_after_dropping_")){

        (*originalRoleList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("role_make_grantable_")){

        (*originalRoleList)[rowIx].isGrantable=modifiedGrantInfo.isGrantable;

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
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("sys_priv_drop_priv_grant_")){

        (*originalSysPrivList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_drop_priv_grant_for_recreation_")){

        (*originalSysPrivList)[rowIx].dropped=true;

    }else if(taskName.startsWith("sys_priv_create_priv_grant_after_dropping_")){

        (*originalSysPrivList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("sys_priv_make_grantable_")){

        (*originalSysPrivList)[rowIx].isGrantable=modifiedGrantInfo.isGrantable;

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

void UserCreatorGrantsAdvancedLayout::quotaAlterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalQuotaList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(quotasTable->table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("quota_add_")){
        Q_ASSERT(originalQuotaList->size()>rowIx);
    }

    TablespaceQuotaInfo modifiedQuotaInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("quota_add_tablespace_quota_")){

        originalQuotaList->append(modifiedQuotaInfo);
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("quota_drop_tablespace_quota_")){

        (*originalQuotaList)[rowIx].dropped=true;

    }else if(taskName.startsWith("quota_alter_tablespace_quota_")){

        (*originalQuotaList)[rowIx]=modifiedQuotaInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalQuotaList);
}

void UserCreatorGrantsAdvancedLayout::quotaAlterQueryError(const QString &)
{
}
