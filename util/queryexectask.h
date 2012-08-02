#ifndef QUERYEXECTASK_H
#define QUERYEXECTASK_H

#include <QString>
#include <QPointer>
#include "param.h"

class QueryExecTask
{
public:
    QueryExecTask();
    QPointer<QObject> requester;
    QString query;
    QList<Param*> params;
    QString taskName;

    QString queryCompletedSlotName;
    QString fetchSlotName;
    QString fetchCompletedSlotName;

    bool retrieveResultsetAsBindParameter;

    QPointer<QObject> queryEndMonitor;
};

#endif // QUERYEXECTASK_H
