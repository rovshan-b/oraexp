#include "dbobjectcomparerfactory.h"
#include "tablecomparehelper.h"
#include "sourcecomparehelper.h"
#include "sequencecomparehelper.h"
#include "util/dbutil.h"

DbObjectComparerFactory::DbObjectComparerFactory()
{
}

DbObjectCompareHelper *DbObjectComparerFactory::createComparerForObjectsType(DbTreeModel::DbTreeNodeType dbObjectsType,
                                                                             const QString &compareResult,
                                                                             const QString &currentObjectName,
                                                                             QObject *parent)
{
    DbObjectCompareHelper *result;

    switch(dbObjectsType){
    case DbTreeModel::Tables:
        result=new TableCompareHelper(compareResult, currentObjectName, parent);
        break;
    case DbTreeModel::Views:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageBodies:
    case DbTreeModel::Procedures:
    case DbTreeModel::Functions:
    case DbTreeModel::Types:
    case DbTreeModel::Synonyms:
        result=new SourceCompareHelper(compareResult, currentObjectName, dbObjectsType, parent);
        break;
    case DbTreeModel::Sequences:
        result=new SequenceCompareHelper(compareResult, currentObjectName, parent);
        break;
    default:
        result=0;
        break;
    }

    return result;
}
