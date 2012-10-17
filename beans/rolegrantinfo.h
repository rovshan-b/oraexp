#ifndef ROLEGRANTINFO_H
#define ROLEGRANTINFO_H

#include <QString>
#include "defines.h"
#include "connectivity/fetchresult.h"

class RoleGrantInfo
{
public:
    RoleGrantInfo();

    int roleId;
    QString roleName;
    bool isGrantable;
    bool isDefault;
    bool markedForDeletion;

    QString generateDdl(const QString &username) const;
    QString generateDropDdl(const QString &username) const;
    QList< NameQueryPair > generateDiffDdl(const RoleGrantInfo &other,
                                           const QString &username) const;
    bool needsRecreation(const RoleGrantInfo &other) const;

    static RoleGrantInfo fromFetchResult(const FetchResult &result);
};

#endif // ROLEGRANTINFO_H
