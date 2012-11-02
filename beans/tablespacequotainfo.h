#ifndef TABLESPACEQUOTAINFO_H
#define TABLESPACEQUOTAINFO_H

#include "enums.h"
#include "defines.h"
#include "connectivity/fetchresult.h"
#include <QString>

class TablespaceQuotaInfo
{
public:
    TablespaceQuotaInfo();

    bool operator==(const TablespaceQuotaInfo &other) const;

    int objId;
    QString tablespaceName;
    qulonglong quota;
    OraExp::ExtentUnit quotaUnit;
    bool unlimitedQuota;

    bool markedForDeletion;
    bool dropped;

    QString generateDdl(const QString &username) const;
    QString generateDropDdl(const QString &username) const;
    QList< NameQueryPair > generateDiffDdl(const TablespaceQuotaInfo &other, const QString &username) const;

    static QString generateListDdl(const QList<TablespaceQuotaInfo> *list,
                                   const QString &username);
    static QList< NameQueryPair > generateListDiffDdl(const QList<TablespaceQuotaInfo> *currents,
                                                      const QList<TablespaceQuotaInfo> *others,
                                                      const QString &username);

    bool needsRecreation(const TablespaceQuotaInfo &other) const;

    static TablespaceQuotaInfo fromFetchResult(const FetchResult &result);

    //QString toString() const;

private:
    QString getQuotaClause(bool unlimitedQuota, uint quota, OraExp::ExtentUnit quotaUnit) const;
};

#endif // TABLESPACEQUOTAINFO_H
