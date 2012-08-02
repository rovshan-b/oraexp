#ifndef TABLEINDEXESLOADER_H
#define TABLEINDEXESLOADER_H

#include <QObject>
#include "beans/indexinfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;
class PartitionInfoLoader;

class TableIndexesLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableIndexesLoader(QObject *parent);

    void loadInfo();

signals:
    void infoReady(const QList<IndexInfo> &tableIndexes);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

    void indexColumnsQueryCompleted(const QueryResult &result);
    void indexColumnsRecordFetched(const FetchResult &result);

    void partitionInfoReady(const TablePartitioningInfo &pInfo, PartitionInfoLoader *loader);
    void paritionInfoLoadError(const QString &taskName, const OciException &ex, PartitionInfoLoader *loader);

private:
    TableInfoLoader *tableInfoLoader;
    QList<IndexInfo> indexes;

    int subqueryCount;

    void decreaseSubqueryCount();

};

#endif // TABLEINDEXESLOADER_H
