#ifndef SEQUENCECOMPAREHELPER_H
#define SEQUENCECOMPAREHELPER_H

#include "dbobjectcomparehelper.h"
#include "beans/sequenceinfo.h"

class DbObjectsCompareHelper;
class MetadataLoader;

class SequenceCompareHelper : public DbObjectCompareHelper
{
public:
    SequenceCompareHelper(const QString &compareResult, const QString &currentObjectName, QObject *parent);
    virtual ~SequenceCompareHelper();


protected:
    virtual QPair<QString,QString> generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options);
    virtual QPair<QString,QString> generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options);

};

#endif // SEQUENCECOMPAREHELPER_H
