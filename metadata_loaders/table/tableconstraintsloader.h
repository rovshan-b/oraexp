#ifndef TABLECONSTRAINTSLOADER_H
#define TABLECONSTRAINTSLOADER_H

#include <QObject>
#include "beans/constraintinfo.h"
#include "connectivity/queryresult.h"

class TableInfoLoader;

class TableConstraintsLoader : public QObject
{
    Q_OBJECT
public:
    explicit TableConstraintsLoader(QObject *parent);

    void loadInfo(bool checkConstraints);

signals:
    void infoReady(const QList<ConstraintInfo> &tableConstraints, bool checkConstraints);
    void loadError(const QString &taskName, const OciException &ex);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordFetched(const FetchResult &result);
    void fetchCompleted(const QString &/*taskName*/);

    void consColNamesQueryCompleted(const QueryResult &result);
    void consColNamesRecordFetched(const FetchResult &result);

private:
    TableInfoLoader *tableInfoLoader;
    QList<ConstraintInfo> constraints;
    bool checkConstraints;

    int fkSubqueryCount;

};

#endif // TABLECONSTRAINTSLOADER_H
