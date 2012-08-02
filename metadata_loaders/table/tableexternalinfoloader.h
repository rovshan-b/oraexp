#ifndef TABLEEXTERNALINFOLOADER_H
#define TABLEEXTERNALINFOLOADER_H

#include <QObject>
#include "beans/tableexternalinfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;

class TableExternalInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableExternalInfoLoader(QObject *parent);

    void loadInfo();

signals:
    void infoReady(const TableExternalInfo &externalInfo);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

    void locationsQueryCompleted(const QueryResult &result);
    void locationsRecordFetched(const FetchResult &result);
    void locationsFetchCompleted(const QString &/*taskName*/);

private:
    TableInfoLoader *tableInfoLoader;
    TableExternalInfo info;
};

#endif // TABLEEXTERNALINFOLOADER_H
