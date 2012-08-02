#include "metadataloader.h"

MetadataLoader::MetadataLoader(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, QObject *parent) :
    QObject(parent), queryScheduler(queryScheduler), schemaName(schemaName), objectName(objectName)
{
}
