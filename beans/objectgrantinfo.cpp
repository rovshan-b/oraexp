#include "objectgrantinfo.h"
#include "util/dbutil.h"

ObjectGrantInfo::ObjectGrantInfo() : objId(-1),
                         markedForDeletion(false),
                         dropped(false)
{
}

bool ObjectGrantInfo::operator==(const ObjectGrantInfo &other) const
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

ObjectGrantInfo ObjectGrantInfo::fromFetchResult(const FetchResult &result, DbTreeModel::DbTreeNodeType objectType)
{
    ObjectGrantInfo inf;

    QStringList privileges=result.colValue("GRANTED_PRIVILEGES").split(',');

    inf.grantee=result.colValue("GRANTEE");
    inf.objectName=QString("\"%1\".\"%2\"").arg(result.colValue("TABLE_SCHEMA"), result.colValue("TABLE_NAME"));
    inf.objectType=objectType;

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames, objectType);

    QString privName;
    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);
        inf.privileges[privName]=getPrivilegeIndex(privName, privileges);
    }

    return inf;
}

int ObjectGrantInfo::getPrivilegeIndex(const QString &privilegeName, const QStringList &privileges)
{
    if(privileges.contains(privilegeName)){
        return 1;
    }else if(privileges.contains(QString("%1 (GRANTABLE)").arg(privilegeName))){
        return 2;
    }else{
        return 0;
    }
}

QList< NameQueryPair > ObjectGrantInfo::generateDdl() const
{
    QList< NameQueryPair > result;

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames, this->objectType);

    QString privName;
    QString privDdl;

    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);

        privDdl=generateGrantStatement(privName, grantee, privileges.value(privName, 0));
        if(!privDdl.isEmpty()){
            result.append(qMakePair(QString("add_table_grant_%1_%2").arg(privName, QString::number(objId)), privDdl));
        }
    }

    return result;
}

QString ObjectGrantInfo::generateRevokeAllDdl() const
{
    return QString("REVOKE ALL ON %1 FROM \"%2\"").arg(objectName, grantee);
}

QList< NameQueryPair > ObjectGrantInfo::generateDiffDdl(const ObjectGrantInfo &other /*tableType*/) const
{
    QList< NameQueryPair > result;

    QString ddl;
    int grantType;
    int otherGrantType;

    if(markedForDeletion && !other.dropped){
        result.append(qMakePair(QString("revoke_all_privileges_%1").arg(objId), other.generateRevokeAllDdl()));
    }else if(!other.dropped){
        QHashIterator<QString,int> i(privileges);
         while (i.hasNext()) {
             i.next();

             grantType=i.value();
             otherGrantType=other.privileges.value(i.key());

             if(grantType!=otherGrantType){
                 result.append(qMakePair(QString("change_grant_type_%1").arg(objId), generateGrantStatement(i.key(), grantee, grantType, true)));
             }
         }
    }

    return result;
}

QString ObjectGrantInfo::generateGrantStatement(const QString &privilegeName,
                                          const QString &granteeName,
                                          int grantType,
                                          bool generateRevoke) const
{
    if(grantType==0 && !generateRevoke){
        return "";
    }

    QString grantStatement;

    if(grantType==1 || grantType==2){
        grantStatement.append("GRANT ").append(privilegeName).append(" ON ").append(objectName)
                      .append(" TO \"").append(granteeName).append("\"");

        if(grantType==2){
            grantStatement.append(" WITH GRANT OPTION");
        }
    }else{
        grantStatement.append("REVOKE ").append(privilegeName).append(" ON ").append(objectName)
                .append(" FROM \"").append(granteeName).append("\"");
    }

    return grantStatement;
}
