#include "rolegrantinfo.h"

RoleGrantInfo::RoleGrantInfo() : roleId(-1), isGrantable(false), isDefault(false), markedForDeletion(false)
{
}

QString RoleGrantInfo::generateDdl(const QString &username) const
{
}

QString RoleGrantInfo::generateDropDdl(const QString &username) const
{
}

QList<NameQueryPair> RoleGrantInfo::generateDiffDdl(const RoleGrantInfo &other, const QString &username) const
{
}

bool RoleGrantInfo::needsRecreation(const RoleGrantInfo &other) const
{
}

RoleGrantInfo RoleGrantInfo::fromFetchResult(const FetchResult &result)
{
}
