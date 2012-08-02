#include "sequencecomparehelper.h"
#include "metadata_loaders/sequence/sequenceinfoloader.h"
#include "dbobjectscomparehelper.h"

SequenceCompareHelper::SequenceCompareHelper(const QString &compareResult, const QString &currentObjectName, QObject *parent) :
    DbObjectCompareHelper(DbTreeModel::Sequences, compareResult, currentObjectName, parent)
{
}

SequenceCompareHelper::~SequenceCompareHelper()
{
    qDebug("deleted SourceCompareHelper");
}

QPair<QString, QString> SequenceCompareHelper::generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options)
{
    SequenceInfo *sourceSequenceInfo=static_cast<SequenceInfo*>(objectInfo);
    Q_ASSERT(sourceSequenceInfo);
    if(sourceSequenceInfo->owner!=dbObjectsCompareHelper->getTargetSchemaName()){
        sourceSequenceInfo->owner=dbObjectsCompareHelper->getTargetSchemaName();
    }

    QPair<QString, QString> result;
    QString ddl=sourceSequenceInfo->generateDdl(options->sequenceCreateOptions);
    result.first=QString("\n%1;").arg(ddl);

    return result;
}

QPair<QString, QString> SequenceCompareHelper::generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options)
{
    SequenceInfo *sourceSequenceInfo=static_cast<SequenceInfo*>(srcObjectInfo);
    SequenceInfo *targetSequenceInfo=static_cast<SequenceInfo*>(targetObjectInfo);

    if(sourceSequenceInfo->owner!=targetSequenceInfo->owner){
        sourceSequenceInfo->owner=targetSequenceInfo->owner;
    }

    Q_ASSERT(sourceSequenceInfo);
    Q_ASSERT(targetSequenceInfo);

    QPair<QString, QString> result;
    QString ddl=sourceSequenceInfo->generateDiffDdl(*targetSequenceInfo, options->sequenceDiffOptions);
    result.first=QString("\n%1;").arg(ddl);

    return result;
}
