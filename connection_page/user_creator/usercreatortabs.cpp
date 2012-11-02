#include "usercreatortabs.h"
#include "tabs/usercreatorgeneralinfo.h"
#include "tabs/usercreatorgrants.h"
#include "tabs/usercreatorgrantsadvancedlayout.h"
#include "util/iconutil.h"
#include "beans/userinfo.h"
#include "beans/dbobjectinfo.h"
#include "metadata_loaders/user/userinfoloader.h"
#include "util/widgethelper.h"
#include "widgets/datatableandtoolbarwidget.h"
#include <QtGui>

UserCreatorTabs::UserCreatorTabs(const QString &objectName, QWidget *parent) :
    SubTabWidget(parent), objectName(objectName), originalUserInfo(0), queryScheduler(0)
{
    editMode = !objectName.isEmpty();

    generalInfoTab = new UserCreatorGeneralInfo(objectName, this, editMode);
    //generalInfoTab->setMaximumWidth(350);
    QScrollArea *scrollAreaForGeneralInfoTab=new QScrollArea();
    scrollAreaForGeneralInfoTab->setWidget(generalInfoTab);
    scrollAreaForGeneralInfoTab->setWidgetResizable(true);
    addTab(scrollAreaForGeneralInfoTab, IconUtil::getIcon("user"), tr("General"));

    QScrollArea *scrollAreaForGrantsTab=new QScrollArea();
    grantsTab = new UserCreatorGrants(this, editMode);
    scrollAreaForGrantsTab->setWidget(grantsTab);
    scrollAreaForGrantsTab->setWidgetResizable(true);
    addTab(scrollAreaForGrantsTab, IconUtil::getIcon("grants"), tr("Grants"));

    connect(generalInfoTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(grantsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
}

void UserCreatorTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    generalInfoTab->setQueryScheduler(queryScheduler);
    grantsTab->setQueryScheduler(queryScheduler);

    if(editMode){
        UserInfoLoader *metadataLoader=new UserInfoLoader(this->queryScheduler, objectName, this);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(userInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }else{
        emit objectInfoLoaded();
    }
}

QString UserCreatorTabs::generateCreateDdl()
{
    return getUserInfo().generateDdl();
}

QList<QueryListItem> UserCreatorTabs::generateAlterDdl()
{
    UserInfo info=getUserInfo();

    QHash<QString,QObject*> requesters;
    requesters["general_info"]=generalInfoTab;
    requesters["role_grants"]=grantsTab->getAdvancedLayout()->getRolesTable();
    requesters["sys_priv_grants"]=grantsTab->getAdvancedLayout()->getSysPrivsTable();
    requesters["quotas"]=grantsTab->getAdvancedLayout()->getQuotasTable();

    return info.generateDiffDdl(*originalUserInfo, requesters);
}

bool UserCreatorTabs::beforeCreate() const
{
    UserInfo info=getUserInfo();
    return WidgetHelper::validate(&info, editMode, this->window());
}

bool UserCreatorTabs::beforeAlter() const
{
    grantsTab->removeIncorrectRows();

    return beforeCreate();
}

QString UserCreatorTabs::getUserName() const
{
    return generalInfoTab->getUserName();
}

UserInfo UserCreatorTabs::getUserInfo() const
{
    UserInfo info;

    info.generalInfo=generalInfoTab->getUserGeneralInfo();
    info.roles=grantsTab->getUserRoles();
    info.sysPrivs=grantsTab->getUserSysPrivs();
    info.quotas=grantsTab->getUserQuotas();

    return info;
}

void UserCreatorTabs::userInfoReady(DbObjectInfo *userInfo, MetadataLoader *loader)
{
    Q_ASSERT(originalUserInfo==0);
    originalUserInfo=static_cast<UserInfo*>(userInfo);

    generalInfoTab->setUserInfo(originalUserInfo);
    grantsTab->setUserInfo(originalUserInfo);

    loader->deleteLater();

    emit objectInfoLoaded();
}

void UserCreatorTabs::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load user information"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();
}
