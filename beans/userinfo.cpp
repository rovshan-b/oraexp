#include "userinfo.h"
#include "util/strutil.h"
#include <QStringList>

UserInfo::UserInfo()
{
}

QString UserInfo::generateDdl() const
{
    QString ddl;

    ddl.append(generalInfo.generateDdl());

    QStringList defaultRoles;
    foreach(const PrivGrantInfo &role, roles){
        ddl.append("\n").append(role.generateDdl(generalInfo.username));
        if(role.isDefault){
            defaultRoles.append(role.name);
        }
    }

    if(defaultRoles.size()>0){
        ddl.append("\n").append(generateDefaultRolesDdl(defaultRoles));
    }

    foreach(const PrivGrantInfo &role, sysPrivs){
        ddl.append("\n").append(role.generateDdl(generalInfo.username));
    }

    return ddl;
}

QString UserInfo::generateDropDdl() const
{
    return generalInfo.generateDropDdl();
}

QList<NameQueryPair> UserInfo::generateDiffDdl(const UserInfo &other) const
{
    QList<NameQueryPair> results;

    results.append(generalInfo.generateDiffDdl(other.generalInfo));
    results.append(PrivGrantInfo::generateListDiffDdl(&roles, &other.roles, generalInfo.username, "role"));

    QStringList defaults=PrivGrantInfo::getDefaults(&roles);
    QStringList otherDefaults=PrivGrantInfo::getDefaults(&other.roles);
    if(defaults!=otherDefaults){
        QString defaultRolesDdl=QString("\n%1").arg(generateDefaultRolesDdl(defaults));
        results.append(qMakePair(QString("role_set_default"), defaultRolesDdl));
    }

    results.append(PrivGrantInfo::generateListDiffDdl(&sysPrivs, &other.sysPrivs, generalInfo.username, "sys_priv"));

    return results;
}

QString UserInfo::generateDefaultRolesDdl(const QStringList &defaultRoles) const
{
    QString ddl;
    ddl.append("ALTER USER \"").append(generalInfo.username).append("\" DEFAULT ROLE ").append(joinEnclosed(defaultRoles, ",", "\""));
    return ddl;
}
