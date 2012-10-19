#include "usergeneralinfoloader.h"
#include "userinfoloader.h"
#include "interfaces/iqueryscheduler.h"
#include "util/param.h"

UserGeneralInfoLoader::UserGeneralInfoLoader(QObject *parent) :
    QObject(parent)
{
    userInfoLoader=static_cast<UserInfoLoader*>(parent);
    Q_ASSERT(userInfoLoader);
}

void UserGeneralInfoLoader::loadInfo()
{
    qDebug("loading UserGeneralInfo");

    userInfoLoader->
            getQueryScheduler()->
            enqueueQuery("get_user_general_info_for_editing",
                         QList<Param*>() << new Param(":owner", userInfoLoader->getObjectName()), this,
                         "get_user_general_info",
                         "queryCompleted", "recordFetched", "fetchCompleted");
}

void UserGeneralInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void UserGeneralInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    generalInfo=UserGeneralInfo::fromFetchResult(result);

    emit infoReady(generalInfo);
}

void UserGeneralInfoLoader::fetchCompleted(const QString &)
{
}
