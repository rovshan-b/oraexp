#include "usercreatorgrantsadvancedlayout.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "beans/userinfo.h"
#include "widgets/userroleseditortable.h"
#include "widgets/usersysprivseditortable.h"
#include "widgets/tablespacequotaseditortable.h"
#include <QtGui>

UserCreatorGrantsAdvancedLayout::UserCreatorGrantsAdvancedLayout(bool editMode, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    tab = new QTabWidget();
    tab->setTabPosition(QTabWidget::South);

    rolesTable = new UserRolesEditorTable(editMode);
    tab->addTab(rolesTable, tr("Roles"));

    sysPrivsTable = new UserSysPrivsEditorTable(editMode);
    tab->addTab(sysPrivsTable, tr("System privileges"));

    quotasTable = new TablespaceQuotasEditorTable(editMode);
    tab->addTab(quotasTable, tr("Quotas"));

    objectPrivTable = new DataTableAndToolBarWidget(0, Qt::Horizontal);
    tab->addTab(objectPrivTable, tr("Object privileges"));

    mainLayout->addWidget(tab);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(rolesTable, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(sysPrivsTable, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(quotasTable, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
}

void UserCreatorGrantsAdvancedLayout::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    rolesTable->setQueryScheduler(queryScheduler);
    sysPrivsTable->setQueryScheduler(queryScheduler);
    quotasTable->setQueryScheduler(queryScheduler);
}

void UserCreatorGrantsAdvancedLayout::setUserInfo(UserInfo *userInfo)
{
    rolesTable->populateTable(&userInfo->roles);
    sysPrivsTable->populateTable(&userInfo->sysPrivs);
    quotasTable->populateTable(&userInfo->quotas);
}

DataTableAndToolBarWidget *UserCreatorGrantsAdvancedLayout::getRolesTable() const
{
    return rolesTable;
}

DataTableAndToolBarWidget *UserCreatorGrantsAdvancedLayout::getSysPrivsTable() const
{
    return sysPrivsTable;
}

DataTableAndToolBarWidget *UserCreatorGrantsAdvancedLayout::getQuotasTable() const
{
    return quotasTable;
}

QList<PrivGrantInfo> UserCreatorGrantsAdvancedLayout::getUserRoles() const
{
    return rolesTable->getList();
}

QList<PrivGrantInfo> UserCreatorGrantsAdvancedLayout::getUserSysPrivs() const
{
    return sysPrivsTable->getList();
}

QList<TablespaceQuotaInfo> UserCreatorGrantsAdvancedLayout::getUserQuotas() const
{
    return quotasTable->getList();
}

void UserCreatorGrantsAdvancedLayout::removeIncorrectRows()
{
    rolesTable->removeIncorrectRows();
    sysPrivsTable->removeIncorrectRows();
    quotasTable->removeIncorrectRows();
}
