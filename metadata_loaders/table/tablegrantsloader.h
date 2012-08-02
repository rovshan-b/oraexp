#ifndef TABLEGRANTSLOADER_H
#define TABLEGRANTSLOADER_H

#include <QObject>
#include "beans/grantinfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;

class TableGrantsLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableGrantsLoader(QObject *parent);
    
    void loadInfo();

signals:
    void infoReady(const QList<GrantInfo> &tableGrants);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    TableInfoLoader *tableInfoLoader;

    QList<GrantInfo> grants;
    
};

#endif // TABLEGRANTSLOADER_H
