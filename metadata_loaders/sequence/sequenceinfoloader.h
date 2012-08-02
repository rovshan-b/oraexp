#ifndef SEQUENCEINFOLOADER_H
#define SEQUENCEINFOLOADER_H

#include <QObject>
#include "../metadataloader.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class IQueryScheduler;

class SequenceInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    explicit SequenceInfoLoader(IQueryScheduler *queryScheduler,
                                const QString &schemaName,
                                const QString &objectName,
                                QObject *parent);

    ~SequenceInfoLoader();

    virtual void loadObjectInfo();

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &);
    
};

#endif // SEQUENCEINFOLOADER_H
