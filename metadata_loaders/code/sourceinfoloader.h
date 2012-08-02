#ifndef SOURCEINFOLOADER_H
#define SOURCEINFOLOADER_H

#include "../metadataloader.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class SourceInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    explicit SourceInfoLoader(IQueryScheduler *queryScheduler,
                              const QString &schemaName,
                              const QString &objectName,
                              const QString &objectType,
                              const QString &targetSchemaName,
                              bool wrap,
                              QObject *parent);

    virtual void loadObjectInfo();
    
private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);

private:
    QString objectType;
    QString targetSchemaName;
    bool wrap;
    
};

#endif // SOURCEINFOLOADER_H
