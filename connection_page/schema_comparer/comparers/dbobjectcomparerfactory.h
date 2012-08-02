#ifndef DBOBJECTCOMPARERFACTORY_H
#define DBOBJECTCOMPARERFACTORY_H

#include "navtree/dbtreemodel.h"

class DbObjectCompareHelper;

class DbObjectComparerFactory
{
private:
    DbObjectComparerFactory();

public:
    static DbObjectCompareHelper *createComparerForObjectsType(DbTreeModel::DbTreeNodeType dbObjectsType, const QString &compareResult, const QString &currentObjectName, QObject *parent);
};

#endif // DBOBJECTCOMPARERFACTORY_H
