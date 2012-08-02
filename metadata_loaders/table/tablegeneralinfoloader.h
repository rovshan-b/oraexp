#ifndef TABLEGENERALINFOLOADER_H
#define TABLEGENERALINFOLOADER_H

#include <QObject>
#include "beans/tablegeneralinfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;

class TableGeneralInfoLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableGeneralInfoLoader(QObject *parent);

    void loadInfo();

signals:
    void infoReady(const TableGeneralInfo &tableGeneralInfo);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    void populateTableDetails(const FetchResult &result);
    void populateIOTDetails(const FetchResult &result);

    TableInfoLoader *tableInfoLoader;
    TableGeneralInfo generalInfo;

    int subqueryCount;

};

#endif // TABLEGENERALINFOLOADER_H
