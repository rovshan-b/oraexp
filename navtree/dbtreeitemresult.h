#ifndef DBTREEITEMRESULT_H
#define DBTREEITEMRESULT_H

#include <QList>
#include <QModelIndex>
#include "connectivity/ociexception.h"

class DbTreeItem;

class DbTreeItemResult
{
public:
    explicit DbTreeItemResult();

    QList<DbTreeItem*> childNodes;
    QModelIndex parentNode;

    bool hasError;
    OciException exception;
};

#endif // DBTREEITEMRESULT_H
