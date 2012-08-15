#include "metadataloaderfactory.h"
#include "metadataloader.h"
#include "interfaces/iqueryscheduler.h"
#include "util/dbutil.h"

#include "table/tableinfoloader.h"
#include "code/sourceinfoloader.h"
#include "sequence/sequenceinfoloader.h"

MetadataLoaderFactory::MetadataLoaderFactory()
{
}

MetadataLoader *MetadataLoaderFactory::createMetadataLoader(DbTreeModel::DbTreeNodeType objectType,
                                                            IQueryScheduler *queryScheduler,
                                                            const QString &schemaName,
                                                            const QString &objectName,
                                                            const QString &targetSchemaName,
                                                            bool wrapCode,
                                                            QObject *parent)
{
    MetadataLoader *result;

    switch(objectType){
    case DbTreeModel::Tables:
        result=new TableInfoLoader(queryScheduler, schemaName, objectName, parent, true);
        break;
    case DbTreeModel::Views:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageBodies:
    case DbTreeModel::Procedures:
    case DbTreeModel::Functions:
    case DbTreeModel::Types:
    case DbTreeModel::Synonyms:
        result=new SourceInfoLoader(queryScheduler, schemaName,
                                    objectName, DbUtil::getDbObjectTypeNameByNodeType(objectType),
                                    targetSchemaName, wrapCode, true, parent);
        break;
    case DbTreeModel::Sequences:
        result=new SequenceInfoLoader(queryScheduler, schemaName, objectName, parent);
        break;
    default:
        result=0;
        break;
    }

    return result;
}
