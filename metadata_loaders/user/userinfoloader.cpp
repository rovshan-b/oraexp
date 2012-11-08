#include "userinfoloader.h"

UserInfoLoader::UserInfoLoader(IQueryScheduler *queryScheduler, const QString &username, QObject *parent) :
    MetadataLoader(queryScheduler, username, username, parent),
    userInfo(0),
    generalInfoLoader(this),
    roleInfoLoader("get_user_roles", this),
    sysPrivInfoLoader("get_user_sys_privs", this),
    quotaInfoLoader(this),
    objectPrivLoader(this)
{
    connect(&generalInfoLoader, SIGNAL(infoReady(UserGeneralInfo)), this, SLOT(userGeneralInfoReady(UserGeneralInfo)));
    connect(&generalInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&roleInfoLoader, SIGNAL(infoReady(QList<PrivGrantInfo>)), this, SLOT(userRoleInfoReady(QList<PrivGrantInfo>)));
    connect(&roleInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&sysPrivInfoLoader, SIGNAL(infoReady(QList<PrivGrantInfo>)), this, SLOT(userSysPrivInfoReady(QList<PrivGrantInfo>)));
    connect(&sysPrivInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&quotaInfoLoader, SIGNAL(infoReady(QList<TablespaceQuotaInfo>)), this, SLOT(tablespaceQuotaInfoReady(QList<TablespaceQuotaInfo>)));
    connect(&quotaInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&objectPrivLoader, SIGNAL(infoReady(QList<ObjectGrantInfo>)), this, SLOT(objectGrantInfoReady(QList<ObjectGrantInfo>)));
    connect(&objectPrivLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));
}

UserInfoLoader::~UserInfoLoader()
{
    qDebug("deleted UserInfoLoader");
}

void UserInfoLoader::loadObjectInfo()
{
    Q_ASSERT(userInfo==0);
    userInfo=new UserInfo();

    partsToLoad.enqueue(UserInfoPartGeneralInfo);
    partsToLoad.enqueue(UserInfoPartRoleInfo);
    partsToLoad.enqueue(UserInfoPartSysPrivInfo);
    partsToLoad.enqueue(UserInfoPartTablespaceQuotaInfo);
    partsToLoad.enqueue(UserInfoPartObjectPrivInfo);

    loadUserPartInfo();
}

void UserInfoLoader::loadUserPartInfo()
{
    if(partsToLoad.isEmpty()){
        emitReadySignal();
        return;
    }

    UserInfoPart part = partsToLoad.dequeue();
    switch(part){
    case UserInfoPartGeneralInfo:
        generalInfoLoader.loadInfo();
        break;
    case UserInfoPartRoleInfo:
        roleInfoLoader.loadInfo();
        break;
    case UserInfoPartSysPrivInfo:
        sysPrivInfoLoader.loadInfo();
        break;
    case UserInfoPartTablespaceQuotaInfo:
        quotaInfoLoader.loadInfo();
        break;
    case UserInfoPartObjectPrivInfo:
        objectPrivLoader.loadInfo(this->queryScheduler,
                                  "get_user_object_grants",
                                  QList<Param*>() << new Param(":owner", this->getObjectName()));
        break;
    default:
        qDebug("Unsupported UserInfoPart");
        break;
    }
}

void UserInfoLoader::userGeneralInfoReady(const UserGeneralInfo &userGeneralInfo)
{
    qDebug("user general info loaded");
    userInfo->generalInfo=userGeneralInfo;

    loadUserPartInfo();
}

void UserInfoLoader::userRoleInfoReady(const QList<PrivGrantInfo> &roles)
{
    qDebug("user role info loaded");
    userInfo->roles=roles;

    loadUserPartInfo();
}

void UserInfoLoader::userSysPrivInfoReady(const QList<PrivGrantInfo> &sysPrivs)
{
    qDebug("user sys priv info loaded");
    userInfo->sysPrivs=sysPrivs;

    loadUserPartInfo();
}

void UserInfoLoader::tablespaceQuotaInfoReady(const QList<TablespaceQuotaInfo> &quotas)
{
    qDebug("user tablespace quota info loaded");
    userInfo->quotas=quotas;

    loadUserPartInfo();
}

void UserInfoLoader::objectGrantInfoReady(const QList<ObjectGrantInfo> &objectGrants)
{
    qDebug("user object grant info loaded");
    userInfo->objectPrivs=objectGrants;

    loadUserPartInfo();
}

void UserInfoLoader::emitReadySignal()
{
    emit objectInfoReady(userInfo, this);
}

void UserInfoLoader::subInfoLoadError(const QString &taskName, const OciException &ex)
{
    delete userInfo;

    emit loadError(taskName, ex, this);
}
