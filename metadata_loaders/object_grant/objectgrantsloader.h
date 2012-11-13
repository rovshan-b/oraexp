#ifndef OBJECTGRANTSLOADER_H
#define OBJECTGRANTSLOADER_H

#include <QObject>
#include "beans/objectgrantinfo.h"
#include "connectivity/queryresult.h"

class Param;

class ObjectGrantsLoader : public QObject
{
    Q_OBJECT
public:
    explicit ObjectGrantsLoader(DbTreeModel::DbTreeNodeType objectType, QObject *parent);
    
    void loadInfo(IQueryScheduler *queryScheduler, const QString &queryName, const QList<Param *> &params);

signals:
    void infoReady(const QList<ObjectGrantInfo> &objectGrants);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    QList<ObjectGrantInfo> grants;

    DbTreeModel::DbTreeNodeType objectType;
    
};

#endif // OBJECTGRANTSLOADER_H
