#ifndef QUERYLISTITEM_H
#define QUERYLISTITEM_H

#include <QList>
#include "defines.h"

class QueryListItem
{
public:
    QueryListItem();
    QueryListItem(QObject *requester, QList< NameQueryPair > queries);

    QObject *requester;
    QList< NameQueryPair > queries;
};

#endif // QUERYLISTITEM_H
