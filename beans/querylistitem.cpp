#include "querylistitem.h"

QueryListItem::QueryListItem() : requester(0)
{
}

QueryListItem::QueryListItem(QObject *requester, QList< NameQueryPair > queries)
 : requester(requester), queries(queries)
{

}
