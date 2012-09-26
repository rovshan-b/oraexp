#include "dblinkinfo.h"
#include <QStringList>

DbLinkInfo::DbLinkInfo() : currentUser(false), shared(false)
{
}

DbLinkInfo::~DbLinkInfo()
{
    qDebug("DbLinkInfo deleted");
}

QString DbLinkInfo::generateDdl() const
{
    if(name.trimmed().isEmpty()){
        return "--Definition is not complete";
    }

    QString ddl;

    ddl.append("CREATE");

    if(shared){
        ddl.append(" SHARED");
    }

    if(owner=="PUBLIC"){
        ddl.append(" PUBLIC");
    }

    ddl.append(" DATABASE LINK ").append(name.trimmed().toUpper());

    if(currentUser){
        ddl.append("\nCONNECT TO CURRENT_USER");
    }else if(!username.trimmed().isEmpty() && !password.trimmed().isEmpty()){
        ddl.append("\nCONNECT TO ").append(username.trimmed().toUpper()).append(" IDENTIFIED BY \"").append(password.trimmed()).append("\"");
    }

    if(shared){
        ddl.append("\n").
        append("AUTHENTICATED BY ").
        append(sharedAuthenticatedBy).
        append(" IDENTIFIED BY \"").
        append(sharedIdentifiedBy).
        append("\"");
    }

    if(!host.trimmed().isEmpty()){
        ddl.append("\nUSING '").append(host.trimmed()).append("'");
    }

    return ddl;
}

QStringList DbLinkInfo::validate() const
{
    QStringList msgs;

    if(name.trimmed().isEmpty()){
        msgs.append(QObject::tr("Database link name must be entered"));
    }

    if(shared && (sharedAuthenticatedBy.trimmed().isEmpty() || sharedIdentifiedBy.trimmed().isEmpty())){
        msgs.append(QObject::tr("Username and password for shared database link must be entered"));
    }

    return msgs;
}

DbLinkInfo DbLinkInfo::fromFetchResult(const FetchResult &result)
{
    DbLinkInfo i;

    i.owner=result.colValue("OWNER");
    i.name=result.colValue("DB_LINK");
    i.username=result.colValue("USERNAME");
    i.password=result.colValue("PASSWORD");
    if(i.username=="CURRENT_USER"){
        i.currentUser=true;
        i.username="";
    }
    i.host=result.colValue("HOST");
    i.sharedAuthenticatedBy=result.colValue("AUTHUSR");
    i.sharedIdentifiedBy=result.colValue("AUTHPWD");
    i.shared=!i.sharedAuthenticatedBy.isEmpty();
    i.isCompleteInfo=result.colValue("IS_COMPLETE", 0)==1;

    return i;
}
