#include "usergeneralinfo.h"

UserGeneralInfo::UserGeneralInfo()
{
}

QString UserGeneralInfo::generateDdl() const
{
    QString ddl;

    ddl.append("CREATE USER \"").append(username).append("\"");
    ddl.append(" IDENTIFIED ").append(getIdentifiedBy());
    ddl.append(getIdentifiedBy());

    if(!defaultTablespace.isEmpty()){
        ddl.append("\nDEFAULT TABLESPACE \"").append(defaultTablespace).append("\"");
    }

    if(!temporaryTablespace.isEmpty()){
        ddl.append("\nTEMPORARY TABLESPACE \"").append(temporaryTablespace).append("\"");
    }

    if(!profile.isEmpty()){
        ddl.append("\nPROFILE \"").append(profile).append("\"");
    }

    if(expirePassword){
        ddl.append("\nPASSWORD EXPIRE");
    }

    if(accountLocked){
        ddl.append("\nACCOUNT LOCKED");
    }

    if(enableEditions){
        ddl.append("\nENABLE EDITIONS");
    }

    return ddl;
}

QString UserGeneralInfo::generateDropDdl() const
{
    return QString("DROP USER \"%1\"").arg(username);
}

QList<NameQueryPair> UserGeneralInfo::generateDiffDdl(const UserGeneralInfo &other) const
{

}

UserGeneralInfo UserGeneralInfo::fromFetchResult(const FetchResult &result)
{
}

QString UserGeneralInfo::getIdentifiedBy() const
{
    QString result;

    switch(identifiedBy){
    case Password:
        result=QString("BY \"%1\"").arg(password);
        break;
    case Externally:
        result="EXTERNALLY";
        break;
    case Globally:
        result="GLOBALLY";
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    if(identifiedBy!=Password){
        if(!dn.isEmpty()){
            result.append("AS '").append(dn).append("'");
        }
    }

    return result;
}
