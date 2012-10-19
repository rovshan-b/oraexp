#ifndef PRIVGRANTINFOLOADER_H
#define PRIVGRANTINFOLOADER_H

#include <QObject>
#include "beans/privgrantinfo.h"
#include "connectivity/queryresult.h"

class UserInfoLoader;

class PrivGrantInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit PrivGrantInfoLoader(const QString &queryName, QObject *parent);
    
    void loadInfo();

signals:
    void infoReady(const QList<PrivGrantInfo> &grants);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    UserInfoLoader *userInfoLoader;
    QList<PrivGrantInfo> grants;

    QString queryName;
};

#endif // PRIVGRANTINFOLOADER_H
