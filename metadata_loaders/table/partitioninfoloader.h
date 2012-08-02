#ifndef PARTITIONINFOLOADER_H
#define PARTITIONINFOLOADER_H

#include <QObject>
#include "connectivity/ociexception.h"
#include "beans/tablepartitioninginfo.h"
#include "connectivity/queryresult.h"

class IQueryScheduler;
class Param;

class PartitionInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit PartitionInfoLoader(QObject *parent);
    ~PartitionInfoLoader();

    void loadPartitionInfo(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, bool isIndex, int data=0);

    bool isPartitioned;
    int data;
signals:
    void partitionInfoReady(const TablePartitioningInfo &pInfo, PartitionInfoLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, PartitionInfoLoader *loader);

private slots:
    void partitioningInfoLoaded(const QueryResult &result);
    void partitionInfoFetched(const FetchResult &fetchResult);
    void partitionInfoFetchCompleted(const QString &);

    void partitionListQueryCompleted(const QueryResult &result);
    void partitionListRecordFetched(const FetchResult &fetchResult);
    void partitionListFetchCompleted(const QString &taskName);

private:
    TablePartitioningInfo partInfo;

    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString objectName;
    bool isIndex;

    QList<Param*> getQueryParams(const QString &schemaName, const QString &objectName, bool isIndex);
    OraExp::PartitionType partitionTypeFromName(const QString &partitionType) const;

};

#endif // PARTITIONINFOLOADER_H
