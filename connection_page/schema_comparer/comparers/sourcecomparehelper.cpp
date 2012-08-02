#include "sourcecomparehelper.h"
#include "dbobjectscomparehelper.h"
#include "util/queryscheduler.h"
#include "beans/sourceinfo.h"

SourceCompareHelper::SourceCompareHelper(const QString &compareResult, const QString &currentObjectName, DbTreeModel::DbTreeNodeType currentObjectType, QObject *parent) :
    DbObjectCompareHelper(currentObjectType, compareResult, currentObjectName, parent)
{
    this->needsTargetObject=false;
}

SourceCompareHelper::~SourceCompareHelper()
{
    qDebug("deleted SourceCompareHelper");
}

QPair<QString, QString> SourceCompareHelper::generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions */*options*/)
{
    SourceInfo *sourceInfo=static_cast<SourceInfo*>(objectInfo);
    Q_ASSERT(sourceInfo);

    QPair<QString, QString> result;
    result.first=QString("\n%1").arg(sourceInfo->source);

    return result;
}

QPair<QString, QString> SourceCompareHelper::generateDiffDdl(DbObjectInfo */*srcObjectInfo*/, DbObjectInfo */*targetObjectInfo*/, const SchemaComparisonOptions */*options*/)
{
    Q_ASSERT(false);

    return QPair<QString, QString>();
}
