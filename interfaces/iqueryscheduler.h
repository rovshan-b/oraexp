#ifndef IQUERYSCHEDULER_H
#define IQUERYSCHEDULER_H

#include "util/param.h"
#include "util/queryexectask.h"

class DbConnection;

class IQueryScheduler
{
public:
    virtual ~IQueryScheduler(){}

    virtual void enqueueQuery(QueryExecTask &task)=0;
    virtual void enqueueQuery(const QString &queryName, QList<Param*> params,
                      QObject *requester, const QString &taskName,
                      const QString &queryCompletedSlotName,
                      const QString &fetchSlotName=QString(),
                      const QString &fetchCompletedSlotName=QString(),
                      bool retrieveResultsetAsBindParameter=false,
                      const QString &dbLinkName=QString())=0;

    virtual DbConnection *getDb() const=0;

    virtual void increaseRefCount(){Q_ASSERT(false);}
    virtual void decreaseRefCount(){Q_ASSERT(false);}
};

#endif // IQUERYSCHEDULER_H
