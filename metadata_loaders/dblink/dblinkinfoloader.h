#ifndef DBLINKINFOLOADER_H
#define DBLINKINFOLOADER_H

#include <QObject>
#include "../metadataloader.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;

class DbLinkInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    explicit DbLinkInfoLoader(IQueryScheduler *queryScheduler,
                                const QString &schemaName,
                                const QString &objectName,
                                QObject *parent);

    ~DbLinkInfoLoader();

    virtual void loadObjectInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);
    
};

#endif // DBLINKINFOLOADER_H
