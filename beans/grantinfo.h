#ifndef GRANTINFO_H
#define GRANTINFO_H

#include <QString>
#include <QStringList>
#include "connectivity/fetchresult.h"
#include "enums.h"
#include "defines.h"
#include <QMetaType>

class TableGrantInfo
{
public:
    TableGrantInfo();

    bool operator==(const TableGrantInfo &other) const;

    int grantId;
    QString grantee;
    QHash<QString,int> privileges;

    bool markedForDeletion;
    bool dropped;

    static TableGrantInfo fromFetchResult(const FetchResult &result);

    QList< NameQueryPair > generateDdl(const QString &fullTableName) const;
    QString generateRevokeAllDdl(const QString &fullTableName) const;
    QList< NameQueryPair > generateDiffDdl(const TableGrantInfo &other,
                                                    const QString &schemaName,
                                                    const QString &tableName,
                                                    OraExp::TableType tableType) const;
    bool needsRecreation(const TableGrantInfo &) const {return false;}
private:
    QString generateGrantStatement(const QString &tableName,
                                   const QString &privilegeName,
                                   const QString &granteeName,
                                   int grantType,
                                   bool generateRevoke=false) const;

    static int getPrivilegeIndex(const QString &privilegeName, const QStringList &privileges);

};

Q_DECLARE_METATYPE(TableGrantInfo)

Q_DECLARE_METATYPE(QList<TableGrantInfo>)

#endif // GRANTINFO_H
