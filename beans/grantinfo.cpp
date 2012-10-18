#include "grantinfo.h"
#include "util/dbutil.h"

TableGrantInfo::TableGrantInfo() : grantId(-1),
                         markedForDeletion(false),
                         dropped(false)
{
}

bool TableGrantInfo::operator==(const TableGrantInfo &other) const
{
    if(!(grantee==other.grantee)){
        return false;
    }

    int grantType;
    int otherGrantType;

    QHashIterator<QString,int> i(privileges);
     while (i.hasNext()) {
         i.next();

         grantType=i.value();
         otherGrantType=other.privileges.value(i.key());

         if(grantType!=otherGrantType){
             return false;
         }
     }

     return true;
}

TableGrantInfo TableGrantInfo::fromFetchResult(const FetchResult &result)
{
    TableGrantInfo inf;

    QStringList privileges=result.colValue("GRANTED_PRIVILEGES").split(',');

    inf.grantee=result.colValue("GRANTEE");

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames);

    QString privName;
    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);
        inf.privileges[privName]=getPrivilegeIndex(privName, privileges);
    }

    return inf;
}

int TableGrantInfo::getPrivilegeIndex(const QString &privilegeName, const QStringList &privileges)
{
    if(privileges.contains(privilegeName)){
        return 1;
    }else if(privileges.contains(QString("%1 (GRANTABLE)").arg(privilegeName))){
        return 2;
    }else{
        return 0;
    }
}

QList< NameQueryPair > TableGrantInfo::generateDdl(const QString &fullTableName) const
{
    QList< NameQueryPair > result;

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames);

    QString privName;
    QString privDdl;

    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);

        privDdl=generateGrantStatement(fullTableName, privName, grantee, privileges.value(privName, 0));
        if(!privDdl.isEmpty()){
            result.append(qMakePair(QString("add_table_grant_%1_%2").arg(privName, QString::number(grantId)), privDdl));
        }
    }

    return result;
}

QString TableGrantInfo::generateRevokeAllDdl(const QString &fullTableName) const
{
    return QString("REVOKE ALL ON %1 FROM \"%2\"").arg(fullTableName, grantee);
}

QList< NameQueryPair > TableGrantInfo::generateDiffDdl(const TableGrantInfo &other,
                                                const QString &schemaName,
                                                const QString &tableName,
                                                OraExp::TableType /*tableType*/) const
{
    QString fullTableName=QString("\"%1\".\"%2\"").arg(schemaName).arg(tableName);

    QList< NameQueryPair > result;

    QString ddl;
    int grantType;
    int otherGrantType;

    if(markedForDeletion && !other.dropped){
        result.append(qMakePair(QString("revoke_all_privileges_%1").arg(grantId), other.generateRevokeAllDdl(fullTableName)));
    }else if(!other.dropped){
        QHashIterator<QString,int> i(privileges);
         while (i.hasNext()) {
             i.next();

             grantType=i.value();
             otherGrantType=other.privileges.value(i.key());

             if(grantType!=otherGrantType){
                 result.append(qMakePair(QString("change_grant_type_%1").arg(grantId), generateGrantStatement(fullTableName, i.key(), grantee, grantType, true)));
             }
         }
    }

    return result;
}

QString TableGrantInfo::generateGrantStatement(const QString &tableName,
                                          const QString &privilegeName,
                                          const QString &granteeName,
                                          int grantType,
                                          bool generateRevoke) const
{
    if(grantType==0 && !generateRevoke){
        return "";
    }

    QString grantStatement;

    if(grantType==1 || grantType==2){
        grantStatement.append("GRANT ").append(privilegeName).append(" ON ").append(tableName)
                      .append(" TO \"").append(granteeName).append("\"");

        if(grantType==2){
            grantStatement.append(" WITH GRANT OPTION");
        }
    }else{
        grantStatement.append("REVOKE ").append(privilegeName).append(" ON ").append(tableName)
                .append(" FROM \"").append(granteeName).append("\"");
    }

    return grantStatement;
}
