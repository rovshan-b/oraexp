#include "queryscheduler.h"
#include "connectivity/dbconnection.h"

QueryScheduler::QueryScheduler()
{
}

void QueryScheduler::enqueueQuery(QueryExecTask &task)
{
    beforeEnqueueQuery();

    task.queryEndMonitor=getQueryEndMonitor();
    Q_ASSERT(task.queryEndMonitor);
    DbConnection *database=getDb();
    Q_ASSERT(database);

    database->enqueueQuery(task);
}

void QueryScheduler::enqueueQuery(const QString &queryName, QList<Param*> params,
                  QObject *requester, const QString &taskName,
                  const QString &queryCompletedSlotName,
                  const QString &fetchSlotName,
                  const QString &fetchCompletedSlotName,
                  bool retrieveResultsetAsBindParameter)
{
    beforeEnqueueQuery();

    QObject *queryEndMonitor=getQueryEndMonitor();
    Q_ASSERT(queryEndMonitor);
    DbConnection *database=getDb();
    Q_ASSERT(database);

    database->enqueueQuery(queryName, params, requester, taskName, queryCompletedSlotName, fetchSlotName, fetchCompletedSlotName,
                     retrieveResultsetAsBindParameter, queryEndMonitor);
}
