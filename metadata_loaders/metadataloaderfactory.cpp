#include "metadataloaderfactory.h"
#include "metadataloader.h"
#include "interfaces/iqueryscheduler.h"
#include "util/dbutil.h"

#include "table/tableinfoloader.h"
#include "code/sourceinfoloader.h"
#include "sequence/sequenceinfoloader.h"
#include "synonym/synonyminfoloader.h"
#include "dblink/dblinkinfoloader.h"

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
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeBodies:
    case DbTreeModel::Synonyms:
        result=new SourceInfoLoader(queryScheduler, schemaName,
                                    objectName, DbUtil::getDbObjectTypeNameByNodeType(objectType),
                                    targetSchemaName, wrapCode, true, parent);
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        result=new SequenceInfoLoader(queryScheduler, schemaName, objectName, parent);
        break;
    //case DbTreeModel::Synonyms: Synonyms is passed when comparing database schemas in the above code
    case DbTreeModel::Synonym:
        result=new SynonymInfoLoader(queryScheduler, schemaName, objectName, parent);
        break;
    case DbTreeModel::DatabaseLinks:
    //case DbTreeModel::PublicDatabaseLinks:
    case DbTreeModel::DatabaseLink:
        result=new DbLinkInfoLoader(queryScheduler, schemaName, objectName, parent);
        break;
    default:
        result=0;
        break;
    }

    return result;
}
