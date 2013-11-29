#ifndef OBJECTRESOLVER_H
#define OBJECTRESOLVER_H

#include <QObject>
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class ObjectResolver : public QObject
{
    Q_OBJECT
public:
    explicit ObjectResolver(QObject *parent = 0);

    void resolveName(const QString &objectName, IQueryScheduler *queryScheduler);

signals:
    void resolved(const QString &objectTypeName,
                  DbTreeModel::DbTreeNodeType objectType,
                  const QString &schemaName,
                  const QString &objectName,
                  ObjectResolver *resolver);
    void resolveError(const OciException &ex,
                      ObjectResolver *resolver);
    
private slots:
    void resolveQueryCompleted(const QueryResult &result);
    void resolveResultReady(const FetchResult &result);
    void resolveFetchCompleted(const QString &taskName);
    
};

#endif // OBJECTRESOLVER_H
