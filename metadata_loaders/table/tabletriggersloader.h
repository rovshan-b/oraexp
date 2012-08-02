#ifndef TABLETRIGGERSLOADER_H
#define TABLETRIGGERSLOADER_H

#include <QObject>
#include "beans/triggerinfo.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class TableInfoLoader;

class TableTriggersLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableTriggersLoader(QObject *parent);
    
    void loadInfo();

signals:
    void infoReady(const QList<TriggerInfo> &tableTriggers);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

private:
    TableInfoLoader *tableInfoLoader;

    QList<TriggerInfo> triggers;
    
};

#endif // TABLETRIGGERSLOADER_H
