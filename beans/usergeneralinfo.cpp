#include "usergeneralinfo.h"

UserGeneralInfo::UserGeneralInfo()
{
}

QString UserGeneralInfo::generateDdl() const
{
    QString ddl;

    ddl.append("CREATE USER \"").append(username).append("\"");
    ddl.append(" IDENTIFIED ").append(getIdentifiedBy());

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
        ddl.append("\nACCOUNT LOCK");
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
    QList< NameQueryPair > result;

    QString ddl;

    if((identifiedBy!=other.identifiedBy || dn!=other.dn) ||
            (identifiedBy==Password && other.identifiedBy==Password && password!=other.password)){
        ddl.append(" IDENTIFIED ").append(getIdentifiedBy());
    }

    if(defaultTablespace!=other.defaultTablespace){
        ddl.append("\nDEFAULT TABLESPACE \"").append(defaultTablespace).append("\"");
    }

    if(temporaryTablespace!=other.temporaryTablespace){
        ddl.append("\nTEMPORARY TABLESPACE \"").append(temporaryTablespace).append("\"");
    }

    if(profile!=other.profile){
        ddl.append("\nPROFILE \"").append(profile).append("\"");
    }

    if(expirePassword && !other.expirePassword){
        ddl.append("\nPASSWORD EXPIRE");
    }

    if(accountLocked!=other.accountLocked){
        ddl.append(accountLocked ? "\nACCOUNT LOCK" : "\nACCOUNT UNLOCK");
    }

    if(enableEditions && !other.enableEditions){
        ddl.append("\nENABLE EDITIONS");
    }

    if(!ddl.isEmpty()){
        QString strToPrepend;
        strToPrepend.append("ALTER USER \"").append(username).append("\" ");
        ddl.prepend(strToPrepend);

        result.append(qMakePair(QString("alter_user"), ddl));
    }

    return result;
}

UserGeneralInfo UserGeneralInfo::fromFetchResult(const FetchResult &result)
{
    UserGeneralInfo info;

    info.username=result.colValue("USERNAME");
    QString authType;
    if(result.columnIndexes.contains("AUTHENTICATION_TYPE")){ //11g or later
        authType=result.colValue("AUTHENTICATION_TYPE");
    }else{ //earlier release
        authType=result.colValue("PASSWORD");
    }

    if(authType=="EXTERNAL"){
        info.identifiedBy=Externally;
    }else if(authType=="GLOBAL"){
        info.identifiedBy=Globally;
    }else{
        info.identifiedBy=Password;
    }

    info.dn=result.colValue("EXTERNAL_NAME");
    info.defaultTablespace=result.colValue("DEFAULT_TABLESPACE");
    info.temporaryTablespace=result.colValue("TEMPORARY_TABLESPACE");
    info.profile=result.colValue("PROFILE");
    info.expirePassword=result.colValue("ACCOUNT_STATUS").contains("EXPIRED");
    info.accountLocked=result.colValue("ACCOUNT_STATUS").contains("LOCKED");
    info.enableEditions=result.colValue("EDITIONS_ENABLED")=="Y";

    return info;
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
            QString escapedDn(dn);
            escapedDn.replace("'","''");
            result.append(" AS '").append(escapedDn).append("'");
        }
    }

    return result;
}
