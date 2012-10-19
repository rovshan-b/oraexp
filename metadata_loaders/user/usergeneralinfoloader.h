#ifndef USERGENERALINFOLOADER_H
#define USERGENERALINFOLOADER_H

#include <QObject>
#include "beans/usergeneralinfo.h"
#include "connectivity/queryresult.h"

class UserInfoLoader;

class UserGeneralInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit UserGeneralInfoLoader(QObject *parent);

    void loadInfo();
    
signals:
    void infoReady(const UserGeneralInfo &userGeneralInfo);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    UserInfoLoader *userInfoLoader;
    UserGeneralInfo generalInfo;
    
};

#endif // USERGENERALINFOLOADER_H
