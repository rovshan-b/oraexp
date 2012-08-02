#ifndef DBOBJECTHASHER_H
#define DBOBJECTHASHER_H

#include <QObject>
#include "connectivity/ociexception.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/schemacomparisonoptions.h"

class IQueryScheduler;

class DbObjectHasher : public QObject
{
    Q_OBJECT
public:
    explicit DbObjectHasher(QObject *parent);

    void createHashForObjects(IQueryScheduler *queryScheduler,
                              const QString &objectType,
                              const QString &schemaName,
                              const QString &targetSchemaName,
                              SchemaComparisonOptions *options,
                              const QStringList &objectNames, int startIx,
                              int count, const QString &hashToCompareWith="");
    
signals:
    void hashReady(const QString &hash);
    void hashError(const QString &taskName, const OciException &exception);
    
private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);
    
};

#endif // DBOBJECTHASHER_H
