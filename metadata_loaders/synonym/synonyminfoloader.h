#ifndef SYNONYMINFOLOADER_H
#define SYNONYMINFOLOADER_H

#include <QObject>
#include "../metadataloader.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;

class SynonymInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    explicit SynonymInfoLoader(IQueryScheduler *queryScheduler,
                                const QString &schemaName,
                                const QString &objectName,
                                QObject *parent);

    ~SynonymInfoLoader();

    virtual void loadObjectInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);
    
};

#endif // SYNONYMINFOLOADER_H
