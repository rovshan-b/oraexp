#ifndef USERINFOLOADER_H
#define USERINFOLOADER_H

#include "connectivity/ociexception.h"
#include "connectivity/queryresult.h"
#include "beans/userinfo.h"
#include "beans/privgrantinfo.h"
#include "util/param.h"
#include "../metadataloader.h"
#include "usergeneralinfoloader.h"
#include "privgrantinfoloader.h"
#include "tablespacequotainfoloader.h"
#include "../object_grant/objectgrantsloader.h"
#include <QQueue>

class UserInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    enum UserInfoPart
    {
        UserInfoPartGeneralInfo,
        UserInfoPartRoleInfo,
        UserInfoPartSysPrivInfo,
        UserInfoPartTablespaceQuotaInfo,
        UserInfoPartObjectPrivInfo
    };

    explicit UserInfoLoader(IQueryScheduler *queryScheduler,
                            const QString &username,
                            QObject *parent);

    virtual ~UserInfoLoader();

    virtual void loadObjectInfo();
    
private slots:
    void userGeneralInfoReady(const UserGeneralInfo &userGeneralInfo);
    void userRoleInfoReady(const QList<PrivGrantInfo> &roles);
    void userSysPrivInfoReady(const QList<PrivGrantInfo> &sysPrivs);
    void tablespaceQuotaInfoReady(const QList<TablespaceQuotaInfo> &quotas);
    void objectGrantInfoReady(const QList<ObjectGrantInfo> &objectGrants);
    
    void emitReadySignal();

    void subInfoLoadError(const QString &taskName, const OciException &ex);

private:
    void loadUserPartInfo();

    UserInfo *userInfo;

    UserGeneralInfoLoader generalInfoLoader;
    PrivGrantInfoLoader roleInfoLoader;
    PrivGrantInfoLoader sysPrivInfoLoader;
    TablespaceQuotaInfoLoader quotaInfoLoader;
    ObjectGrantsLoader objectPrivLoader;

    QQueue<UserInfoPart> partsToLoad;
};

#endif // USERINFOLOADER_H
