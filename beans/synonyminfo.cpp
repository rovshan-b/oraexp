#include "synonyminfo.h"
#include <QStringList>

SynonymInfo::SynonymInfo() : dropped(false)
{
}

QString SynonymInfo::generateDdl(bool replace) const
{
    QString ddl;

    ddl.append("CREATE");
    if(replace){
        ddl.append(" OR REPLACE");
    }

    if(owner=="PUBLIC"){
        ddl.append(" PUBLIC");
    }

    ddl.append(" SYNONYM ");

    if(owner!="PUBLIC"){
        ddl.append("\"").append(owner).append("\".");
    }

    ddl.append("\"").append(name).append("\"");
    ddl.append(" FOR \"").append(targetSchema).append("\".\"").append(targetObject).append("\"");

    if(!dbLinkName.isEmpty()){
        ddl.append("@\"").append(dbLinkName).append("\"");
    }

    ddl.append(";");

    return ddl;
}

QString SynonymInfo::generateDropDdl() const
{
    QString ddl;

    ddl.append("DROP");
    if(owner=="PUBLIC"){
        ddl.append(" PUBLIC");
    }

    ddl.append(" SYNONYM ");

    if(owner!="PUBLIC"){
        ddl.append("\"").append(owner).append("\".");
    }

    ddl.append("\"").append(name).append("\"");

    return ddl;
}

bool SynonymInfo::needsRecreation(const SynonymInfo &other) const
{
    return owner!=other.owner;
}

QList<NameQueryPair> SynonymInfo::generateDiffDdl(const SynonymInfo &other) const
{
    QList<NameQueryPair> result;
    QString ddl;

    if(needsRecreation(other)){
        if(!dropped){
            result.append(qMakePair(QString("drop_synonym"), generateDropDdl()));
        }

        result.append(qMakePair(QString("create_synonym"), generateDdl(false)));
    }else{
        if(name!=other.name){
            ddl=QString().append("RENAME \"")
                    .append(other.name)
                    .append("\" TO \"")
                    .append(name)
                    .append("\"");

            result.append(qMakePair(QString("rename_synonym"), ddl));
        }

        if(targetSchema!=other.targetSchema ||
                targetObject!=other.targetObject ||
                dbLinkName!=other.dbLinkName){
            result.append(qMakePair(QString("recreate_synonym"), ddl));
        }
    }

    return result;
}


QStringList SynonymInfo::validate() const
{
    QStringList msgs;

    if(owner!="PUBLIC" && owner.isEmpty()){
        msgs.append(QObject::tr("Schema name for non-public synonym must be entered"));
    }

    if(name.isEmpty()){
        msgs.append(QObject::tr("Synonym name must be entered"));
    }

    if(overDbLink && dbLinkName.isEmpty()){
        msgs.append(QObject::tr("Database link name must be enetered"));
    }

    if(targetSchema.isEmpty() || targetObject.isEmpty()){
        msgs.append(QObject::tr("Target schema and object name must be enetered"));
    }

    return msgs;
}

SynonymInfo SynonymInfo::fromFetchResult(const FetchResult &result)
{
    SynonymInfo i;

    i.owner=result.colValue("OWNER");
    i.name=result.colValue("SYNONYM_NAME");
    i.targetSchema=result.colValue("TABLE_OWNER");
    i.targetObject=result.colValue("TABLE_NAME");
    i.dbLinkName=result.colValue("DB_LINK");

    return i;
}
