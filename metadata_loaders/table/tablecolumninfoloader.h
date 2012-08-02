#ifndef TABLECOLUMNINFOLOADER_H
#define TABLECOLUMNINFOLOADER_H

#include <QObject>
#include "beans/columninfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;

class TableColumnInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableColumnInfoLoader(QObject *parent);

    void loadInfo();

signals:
    void infoReady(const QList<ColumnInfo> &tableColumns);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

    void lobInfoQueryCompleted(const QueryResult &result);
    void lobInfoRecordFetched(const FetchResult &result);

private:
    TableInfoLoader *tableInfoLoader;
    QList<ColumnInfo> columns;

    int lobSubqueryCount;

};

#endif // TABLECOLUMNINFOLOADER_H
