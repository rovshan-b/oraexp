#ifndef SOURCECOMPAREHELPER_H
#define SOURCECOMPAREHELPER_H

#include "dbobjectcomparehelper.h"
#include "metadata_loaders/code/sourceinfoloader.h"

class SourceCompareHelper : public DbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit SourceCompareHelper(const QString &compareResult,
                                 const QString &currentObjectName,
                                 DbTreeModel::DbTreeNodeType currentObjectType,
                                 QObject *parent);
    virtual ~SourceCompareHelper();

protected:
    virtual QPair<QString,QString> generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options);
    virtual QPair<QString,QString> generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options);
};

#endif // SOURCECOMPAREHELPER_H
