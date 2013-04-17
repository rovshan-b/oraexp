#include "triggerinfo.h"
#include "util/strutil.h"

TriggerInfo::TriggerInfo() : isEmpty(true)
{
}

QString TriggerInfo::generateDdl(const QString &fullTableName, bool sqlTerminator) const
{
    QString ddl;

    ddl.append("CREATE OR REPLACE TRIGGER \"").append(owner).append("\".\"").append(name).append("\"");
    ddl.append("\n").append(type).append(" ").append(triggeringEvent);
    if(!columnName.isEmpty()){
        ddl.append(" OF ").append(columnName);
    }

    ddl.append(" ON ").append(baseObjectType=="DATABASE" ? "DATABASE" : fullTableName).append("\n");

    if(referencingNames!="REFERENCING NEW AS NEW OLD AS OLD"){
        ddl.append(referencingNames).append("\n");
    }

    if(!crossEdition.isEmpty() && crossEdition!="NO"){
        ddl.append(crossEdition).append(" CROSSEDITION\n");
    }

    if(forEachRow){
        ddl.append("FOR EACH ROW\n");
    }

    if(!precedes.isEmpty()){
        ddl.append("PRECEDES ").append(precedes).append("\n");
    }

    if(!follows.isEmpty()){
        ddl.append("FOLLOWS ").append(follows).append("\n");
    }

    if(!enabled){
        ddl.append("DISABLE\n");
    }

    if(!whenClause.isEmpty()){
        ddl.append("WHEN (").append(whenClause).append(")\n");
    }

    if(actionType=="CALL"){
        ddl.append(actionType).append(" ");
    }

    ddl.append(body);

    if(sqlTerminator){
        ddl.append("\n").append("/");
    }

    return ddl;
}

QString TriggerInfo::generateDropDdl() const
{
    return QString("DROP TRIGGER \"%1\".\"%2\";").arg(owner, name);
}

QString TriggerInfo::generateDiffDdl(const TriggerInfo &other, const QString &fullTableName) const
{
    QString ddl;

    if(owner!=other.owner ||
        name!=other.name ||
        type!=other.type ||
        triggeringEvent!=other.triggeringEvent ||
        forEachRow!=other.forEachRow ||
        columnName!=other.columnName ||
        referencingNames!=other.referencingNames ||
        whenClause!=other.whenClause ||
        enabled!=other.enabled ||
        follows!=other.follows ||
        actionType!=other.actionType ||
        body!=other.body){

        ddl=generateDdl(fullTableName, true);
    }

    return ddl;
}

TriggerInfo TriggerInfo::fromFetchResult(const FetchResult &result)
{
    TriggerInfo i;

    i.owner=result.colValue("OWNER");
    i.name=result.colValue("TRIGGER_NAME");

    QString triggerType=result.colValue("TRIGGER_TYPE");
    Q_ASSERT(triggerType.size());

    i.type=triggerType.split(' ').at(0);
    if(i.type=="INSTEAD"){
        i.type.append(" OF");
    }
    i.forEachRow=triggerType.contains("EACH ROW");

    i.triggeringEvent=result.colValue("TRIGGERING_EVENT");
    i.baseObjectType=result.colValue("BASE_OBJECT_TYPE").trimmed();
    i.columnName=result.colValue("COLUMN_NAME");
    i.referencingNames=result.colValue("REFERENCING_NAMES");
    i.crossEdition=result.colValue("CROSSEDITION");
    i.precedes=result.colValue("PRECEDES");
    i.follows=result.colValue("FOLLOWS");
    i.whenClause=result.colValue("WHEN_CLAUSE");
    i.enabled=(result.colValue("STATUS")=="ENABLED");
    i.follows=result.colValue("FOLLOWS");
    //i.description=result.colValue("DESCRIPTION");
    i.actionType=result.colValue("ACTION_TYPE").trimmed();
    i.body=result.colValue("TRIGGER_BODY");

    i.isEmpty=false;

    return i;
}
