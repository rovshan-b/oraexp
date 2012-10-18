#include "privgrantinfo.h"

PrivGrantInfo::PrivGrantInfo() :
    grantId(-1),
    isGrantable(false),
    isDefault(false),
    markedForDeletion(false),
    dropped(false)
{
}

bool PrivGrantInfo::operator ==(const PrivGrantInfo &other) const
{
    return name==other.name && isGrantable==other.isGrantable && isDefault==other.isDefault;
}

QString PrivGrantInfo::generateDdl(const QString &username) const
{
    QString ddl;

    ddl.append("GRANT \"").append(name).append("\" TO \"").append(username).append("\"");

    if(isGrantable){
        ddl.append(" WITH ADMIN OPTION");
    }

    return ddl;
}

QString PrivGrantInfo::generateDropDdl(const QString &username) const
{
    return QString("REVOKE \"").append(name).append("\" FROM \"").append(username).append("\"");
}

QList<NameQueryPair> PrivGrantInfo::generateDiffDdl(const PrivGrantInfo &other, const QString &username) const
{
    QList< NameQueryPair > result;

    QString ddl;

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(username);
        result.append(qMakePair(QString("drop_priv_grant_%1").arg(grantId), ddl));

        return result;
    }else if(needsRecreation(other)){
        ddl=other.generateDropDdl(username);
        result.append(qMakePair(QString("drop_priv_grant_for_recreation_%1").arg(grantId), ddl));

        ddl=generateDdl(username);
        result.append(qMakePair(QString("create_priv_grant_after_dropping_%1").arg(grantId), ddl));
    }

    return result;
}

bool PrivGrantInfo::needsRecreation(const PrivGrantInfo &other) const
{
    return (this->isGrantable==true && other.isGrantable==false);
}

PrivGrantInfo PrivGrantInfo::fromFetchResult(const FetchResult &result)
{
    PrivGrantInfo info;

    info.name=result.colValue("PRIVILEGE");
    info.isGrantable=result.colValue("ADMIN_OPTION")=="YES";
    info.isDefault=result.colValue("IS_DEFAULT")=="YES";

    return info;
}
