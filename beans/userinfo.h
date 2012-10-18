#ifndef USERINFO_H
#define USERINFO_H

#include "dbobjectinfo.h"
#include "usergeneralinfo.h"
#include "privgrantinfo.h"

#include <QString>
#include <QList>

class UserInfo : public DbObjectInfo
{
public:
    UserInfo();

    UserGeneralInfo generalInfo;
    QList<PrivGrantInfo> roles;
    QList<PrivGrantInfo> sysPrivs;

    QString generateDdl() const;
    QString generateDropDdl() const;
    QList< NameQueryPair > generateDiffDdl(const UserInfo &other) const;
    static UserInfo fromFetchResult(const FetchResult &result);

};

#endif // USERINFO_H
