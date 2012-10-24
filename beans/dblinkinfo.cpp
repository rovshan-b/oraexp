#include "dblinkinfo.h"
#include <QStringList>
#include "util/strutil.h"

DbLinkInfo::DbLinkInfo() : currentUser(false), shared(false), dropped(false)
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

    ddl.append(" DATABASE LINK \"").append(name.trimmed().toUpper()).append("\"");

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

QString DbLinkInfo::generateDropDdl() const
{
    return QString("DROP%1 DATABASE LINK \"%2\"").arg(owner=="PUBLIC" ? " PUBLIC" : "", name);
}

bool DbLinkInfo::needsRecreation(const DbLinkInfo &other, bool canAlter) const
{
    bool hasDifferences = owner!=other.owner ||
            name!=other.name ||
            currentUser!=other.currentUser ||
            username!=other.username ||
            host!=other.host ||
            shared!=other.shared ||
            sharedAuthenticatedBy!=other.sharedAuthenticatedBy;

    bool passwordsChanged = password!=other.password ||
            sharedIdentifiedBy!=other.sharedIdentifiedBy;

    return (passwordsChanged && !canAlter) || hasDifferences;
}


QList<NameQueryPair> DbLinkInfo::generateDiffDdl(const DbLinkInfo &other, bool canAlter) const
{
    QList<NameQueryPair> result;
    QString ddl;

    if(needsRecreation(other, canAlter)){
        if(!dropped){
            ddl=other.generateDropDdl();
            result.append(qMakePair(QString("drop_db_link"), ddl));
        }

        ddl=generateDdl();
        result.append(qMakePair(QString("create_db_link"), ddl));
    }else{

        if(password!=other.password){
            ddl.append("CONNECT TO ").append(username).append(" IDENTIFIED BY \"").append(password).append("\"");
        }

        if(sharedIdentifiedBy!=other.sharedIdentifiedBy){
            addEOL(ddl);
            ddl.append("AUTHENTICATED BY ").append(sharedAuthenticatedBy).append(" IDENTIFIED BY ").append(sharedIdentifiedBy);
        }

        if(!ddl.isEmpty()){
            ddl.prepend(QString("ALTER%1%2 DATABASE LINK \"%3\"\n").arg(shared ? " SHARED" : "", owner=="PUBLIC" ? " PUBLIC" : ""));
            result.append(qMakePair(QString("alter_db_link"), ddl));
        }
    }

    return result;
}

QStringList DbLinkInfo::validate(bool) const
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
