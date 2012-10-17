#ifndef USERINFO_H
#define USERINFO_H

#include "dbobjectinfo.h"
#include "usergeneralinfo.h"
#include "rolegrantinfo.h"
#include "sysprivgrantinfo.h"

#include <QString>
#include <QList>

class UserInfo : public DbObjectInfo
{
public:
    UserInfo();

    UserGeneralInfo generalInfo;
    QList<RoleGrantInfo> grants;
    QList<SysPrivGrantInfo> sysPrivs;

};

#endif // USERINFO_H
