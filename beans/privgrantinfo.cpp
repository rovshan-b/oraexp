#include "privgrantinfo.h"
#include <QStringList>

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

    ddl.append("GRANT ").append(name).append(" TO \"").append(username).append("\"");

    if(isGrantable){
        ddl.append(" WITH ADMIN OPTION");
    }

    return ddl;
}

QString PrivGrantInfo::generateDropDdl(const QString &username) const
{
    return QString("REVOKE ").append(name).append(" FROM \"").append(username).append("\"");
}

QList<NameQueryPair> PrivGrantInfo::generateDiffDdl(const PrivGrantInfo &other, const QString &username, const QString &taskNamePrefix) const
{
    QList< NameQueryPair > result;

    QString ddl;

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(username);
        result.append(qMakePair(QString("%1_drop_priv_grant_%2").arg(taskNamePrefix, QString::number(grantId)), ddl));

        return result;
    }else if(needsRecreation(other)){
        ddl=other.generateDropDdl(username);
        result.append(qMakePair(QString("%1_drop_priv_grant_for_recreation_%2").arg(taskNamePrefix, QString::number(grantId)), ddl));

        ddl=generateDdl(username);
        result.append(qMakePair(QString("%1_create_priv_grant_after_dropping_%2").arg(taskNamePrefix, QString::number(grantId)), ddl));
    }

    return result;
}

QList<NameQueryPair> PrivGrantInfo::generateListDiffDdl(const QList<PrivGrantInfo> *currents,
                                                        const QList<PrivGrantInfo> *others,
                                                        const QString &username,
                                                        const QString &taskNamePrefix)
{
    QList< NameQueryPair > result;

    Q_ASSERT(others->size()<=currents->size());

    int rowCount=currents->size();

    for(int i=0; i<rowCount; ++i){
        const PrivGrantInfo &grantInfo=currents->at(i);

        if(i<others->size()){
            const PrivGrantInfo &originalGrantInfo=others->at(i);
            QList< NameQueryPair > alterDdls=grantInfo.generateDiffDdl(originalGrantInfo, username, taskNamePrefix);
            result.append(alterDdls);
        }else{
            result.append(qMakePair(QString("%1_add_priv_grant_%2").arg(taskNamePrefix, QString::number(i+1)), grantInfo.generateDdl(username)));
        }
    }

    return result;
}

QStringList PrivGrantInfo::getDefaults(const QList<PrivGrantInfo> *list)
{
    QStringList results;

    for(int i=0; i<list->size(); ++i){
        const PrivGrantInfo &grant=list->at(i);
        if(grant.isDefault){
            results.append(grant.name);
        }
    }

    return results;
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
