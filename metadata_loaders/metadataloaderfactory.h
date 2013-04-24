#ifndef METADATALOADERFACTORY_H
#define METADATALOADERFACTORY_H

#include "navtree/dbtreemodel.h"
#include "table/tableinfoloader.h"
#include "code/sourceinfoloader.h"
#include "sequence/sequenceinfoloader.h"
#include "synonym/synonyminfoloader.h"
#include "dblink/dblinkinfoloader.h"

class MetadataLoader;
class IQueryScheduler;

class MetadataLoaderFactory
{
public:
    MetadataLoaderFactory();

    static MetadataLoader *createMetadataLoader(DbTreeModel::DbTreeNodeType objectType,
                                                IQueryScheduler *queryScheduler,
                                                const QString &schemaName,
                                                const QString &objectName,
                                                const QString &targetSchemaName,
                                                bool wrapCode, QObject *parent);
};

#endif // METADATALOADERFACTORY_H
