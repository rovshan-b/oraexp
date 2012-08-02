#ifndef TABLEEXTERNALINFO_H
#define TABLEEXTERNALINFO_H

#include <QList>
#include "connectivity/fetchresult.h"
#include "defines.h"
#include <QMetaType>

class TableExternalInfo
{
public:
    TableExternalInfo();

    QString driverType;
    QString defaultDirectory;

    int rejectLimit;
    bool rejectLimitUnlimited;

    int projectColumn;
    bool usingClobSubquery;

    int parallel;
    int parallelDegree;

    QString accessParameters;

    QList< StringPair > locations;

    static TableExternalInfo fromFetchResult(const FetchResult &result);

    QString generateDdl() const;
    QList<NameQueryPair> generateAlterDdl(const QString &fullTableName, const TableExternalInfo &originalInfo) const;

    QString generateProjectColumnDdl(const QString &fullTableName) const;

private:
    QList< NameQueryPair > generateDiffDdl(const TableExternalInfo &other, const QString &fullTableName) const;
};

Q_DECLARE_METATYPE(TableExternalInfo)

#endif // TABLEEXTERNALINFO_H
