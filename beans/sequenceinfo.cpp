#include "sequenceinfo.h"

SequenceInfo::SequenceInfo() : dropped(false)
{
}

SequenceInfo::~SequenceInfo()
{
    qDebug("SequenceInfo deleted");
}

QString SequenceInfo::generateDdl(const SequenceCreateDdlOptions &options) const
{
    if(owner.trimmed().isEmpty() || name.trimmed().isEmpty()){
        return QObject::tr("--Definition is not complete. Sequence owner and name must be entered.");
    }

    QString ddl;

    ddl.append(QString("CREATE SEQUENCE \"%1\".\"%2\"").arg(owner, name));

    if(options.setInitialValue && !lastNumber.isEmpty()){
        ddl.append(" START WITH ").append(lastNumber);
    }

    if(!incrementBy.isEmpty()){
        ddl.append(" INCREMENT BY ").append(incrementBy);
    }

    if(!minValue.isEmpty()){
        ddl.append(" MINVALUE ").append(minValue);
    }

    if(!maxValue.isEmpty()){
        ddl.append(" MAXVALUE ").append(maxValue);
    }

    if(cycle){
        ddl.append(" CYCLE");
    }

    if(!cacheSize.isEmpty()){
        if(cacheSize=="0"){
            ddl.append(" NOCACHE");
        }else{
            ddl.append(" CACHE ").append(cacheSize);
        }
    }

    if(ordered){
        ddl.append(" ORDER");
    }

    return ddl;
}

QString SequenceInfo::generateDropDdl() const
{
    return QString("DROP SEQUENCE \"%1\".\"%2\"").arg(owner, name);
}

bool SequenceInfo::needsRecreation(const SequenceInfo &other) const
{
    return lastNumber!=other.lastNumber;
}


QList<NameQueryPair> SequenceInfo::generateDiffDdl(const SequenceInfo &other, const SequenceDiffDdlOptions &options) const
{
    QList<NameQueryPair> result;
    QString ddl;

    if(options.updateCurrVal && needsRecreation(other)){
        if(!dropped){
            ddl=other.generateDropDdl();
            result.append(qMakePair(QString("drop_sequence"), ddl));
        }

        SequenceCreateDdlOptions createOptions;
        createOptions.setInitialValue=true;
        ddl=generateDdl(createOptions);
        result.append(qMakePair(QString("create_sequence"), ddl));
    }else{

        if(name!=other.name){
            ddl=QString().append("RENAME \"")
                    .append(other.name)
                    .append("\" TO \"")
                    .append(name)
                    .append("\"");

            result.append(qMakePair(QString("rename_sequence"), ddl));
        }

        ddl="";
        if(incrementBy!=other.incrementBy){
            ddl.append(" INCREMENT BY ").append(incrementBy);
        }

        if(minValue!=other.minValue){
            if(minValue.isEmpty()){
                ddl.append(" NOMINVALUE");
            }else{
                ddl.append(" MINVALUE ").append(minValue);
            }
        }

        if(maxValue!=other.maxValue){
            if(maxValue.isEmpty()){
                ddl.append(" NOMAXVALUE");
            }else{
                ddl.append(" MAXVALUE ").append(maxValue);
            }
        }

        if(cycle!=other.cycle){
            ddl.append(cycle ? " CYCLE" : " NOCYCLE");
        }

        if(cacheSize!=other.cacheSize){
            if(cacheSize=="0"){
                ddl.append(" NOCACHE");
            }else{
                ddl.append(" CACHE ").append(cacheSize);
            }
        }

        if(ordered!=other.ordered){
            ddl.append(ordered ? " ORDER" : " NOORDER");
        }

        if(!ddl.isEmpty()){
            ddl.prepend(QString("ALTER SEQUENCE \"%1\".\"%2\"").arg(owner, name));
            result.append(qMakePair(QString("alter_sequence"), ddl));
        }
    }

    return result;
}

SequenceInfo SequenceInfo::fromFetchResult(const FetchResult &result)
{
    SequenceInfo i;

    i.owner=result.colValue("SEQUENCE_OWNER");
    i.name=result.colValue("SEQUENCE_NAME");
    i.minValue=result.colValue("MIN_VALUE");
    i.maxValue=result.colValue("MAX_VALUE");
    i.incrementBy=result.colValue("INCREMENT_BY");
    i.cycle=result.colValue("CYCLE_FLAG")=="Y";
    i.ordered=result.colValue("ORDER_FLAG")=="Y";
    i.cacheSize=result.colValue("CACHE_SIZE");
    i.lastNumber=result.colValue("LAST_NUMBER");

    return i;
}
