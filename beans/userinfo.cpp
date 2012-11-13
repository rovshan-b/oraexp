#include "userinfo.h"
#include "util/strutil.h"
#include "code_generators/ddl/tablegrantsddlgenerator.h"
#include <QStringList>

UserInfo::UserInfo()
{
}

QString UserInfo::generateDdl() const
{
    if(generalInfo.username.isEmpty()){
        return "--Definition is not complete. Username must be entered.";
    }

    QString ddl;

    ddl.append(generalInfo.generateDdl()).append(";");

    QStringList defaultRoles;
    foreach(const PrivGrantInfo &role, roles){
        addEOL(ddl);
        ddl.append("\n").append(role.generateDdl(generalInfo.username)).append(";");
        if(role.isDefault){
            defaultRoles.append(role.name);
        }
    }

    if(defaultRoles.size()>0){
        addEOL(ddl);
        ddl.append("\n").append(generateDefaultRolesDdl(defaultRoles)).append(";");
    }

    foreach(const PrivGrantInfo &priv, sysPrivs){
        addEOL(ddl);
        ddl.append("\n").append(priv.generateDdl(generalInfo.username)).append(";");
    }

    QString quotasDdl=TablespaceQuotaInfo::generateListDdl(&this->quotas, generalInfo.username);
    if(!quotasDdl.isEmpty()){
        ddl.append("\n").append(quotasDdl);
    }

    QString objectPrivsDdl=TableGrantsDdlGenerator::generateDdl(&this->objectPrivs);
    if(!objectPrivsDdl.isEmpty()){
        ddl.append("\n").append(objectPrivsDdl);
    }

    return ddl;
}

QString UserInfo::generateDropDdl() const
{
    return generalInfo.generateDropDdl();
}

QList<QueryListItem> UserInfo::generateDiffDdl(const UserInfo &other, const QHash<QString,QObject*> &requesters) const
{
    QList<QueryListItem> results;

    results.append(QueryListItem(requesters.value("general_info"), generalInfo.generateDiffDdl(other.generalInfo)));
    results.append(QueryListItem(requesters.value("role_grants"),
                                 PrivGrantInfo::generateListDiffDdl(&roles, &other.roles, generalInfo.username, "role")));

    QStringList defaults=PrivGrantInfo::getDefaults(&roles);
    QStringList otherDefaults=PrivGrantInfo::getDefaults(&other.roles);
    if(defaults!=otherDefaults){
        QString defaultRolesDdl=generateDefaultRolesDdl(defaults);

        results.append(QueryListItem(requesters.value("role_grants"),
                                     QList<NameQueryPair>() << qMakePair(QString("role_set_default"), defaultRolesDdl)));
    }

    results.append(QueryListItem(requesters.value("sys_priv_grants"),
                                 PrivGrantInfo::generateListDiffDdl(&sysPrivs, &other.sysPrivs, generalInfo.username, "sys_priv")));

    results.append(QueryListItem(requesters.value("quotas"),
                                 TablespaceQuotaInfo::generateListDiffDdl(&quotas, &other.quotas, generalInfo.username)));

    results.append(QueryListItem(requesters.value("object_privs"),
                                 TableGrantsDdlGenerator::generateAlterDdl(&objectPrivs, &other.objectPrivs)));

    return results;
}

QString UserInfo::generateDefaultRolesDdl(const QStringList &defaultRoles) const
{
    QString ddl;
    ddl.append("ALTER USER \"").append(generalInfo.username).append("\" DEFAULT ROLE ").append(joinEnclosed(defaultRoles, ",", "\""));
    return ddl;
}

QStringList UserInfo::validate(bool editMode) const
{
    QStringList msgs;

    if(generalInfo.username.isEmpty()){
        msgs.append(QObject::tr("Username must be entered"));
    }

    if(!editMode &&
            generalInfo.identifiedBy==UserGeneralInfo::Password &&
            generalInfo.password.isEmpty()){
        msgs.append(QObject::tr("Password must be entered"));
    }

    return msgs;
}
