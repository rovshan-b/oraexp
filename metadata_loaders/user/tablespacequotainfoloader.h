#ifndef TABLESPACEQUOTAINFOLOADER_H
#define TABLESPACEQUOTAINFOLOADER_H

#include <QObject>
#include "beans/tablespacequotainfo.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class UserInfoLoader;

class TablespaceQuotaInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit TablespaceQuotaInfoLoader(QObject *parent);
    
    void loadInfo();

signals:
    void infoReady(const QList<TablespaceQuotaInfo> &quotas);
    void loadError(const QString &taskName, const OciException &ex);
    
private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    UserInfoLoader *userInfoLoader;
    QList<TablespaceQuotaInfo> quotas;
    
};

#endif // TABLESPACEQUOTAINFOLOADER_H
