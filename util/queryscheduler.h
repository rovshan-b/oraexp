#ifndef QUERYSCHEDULER_H
#define QUERYSCHEDULER_H

#include "interfaces/iqueryscheduler.h"

class QueryScheduler : public IQueryScheduler
{
public:
    QueryScheduler();

    virtual void enqueueQuery(QueryExecTask &task);
    virtual void enqueueQuery(const QString &queryName, QList<Param*> params,
                      QObject *requester, const QString &taskName,
                      const QString &queryCompletedSlotName,
                      const QString &fetchSlotName=QString(),
                      const QString &fetchCompletedSlotName=QString(),
                      bool retrieveResultsetAsBindParameter=false,
                      const QString &dbLinkName=QString(),
                      bool isUserQuery=false);

    virtual QObject *getQueryEndMonitor() = 0;

protected:
    virtual void beforeEnqueueQuery()=0;
};

#endif // QUERYSCHEDULER_H
