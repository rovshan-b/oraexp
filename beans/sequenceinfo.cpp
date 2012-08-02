#include "sequenceinfo.h"

SequenceInfo::SequenceInfo()
{
}

SequenceInfo::~SequenceInfo()
{
    qDebug("SequenceInfo deleted");
}

QString SequenceInfo::generateDdl(const SequenceCreateDdlOptions &options) const
{
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

    ddl.append(cycle ? " CYCLE" : " NOCYCLE");

    if(!cacheSize.isEmpty()){
        if(cacheSize==0){
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

QString SequenceInfo::generateDiffDdl(const SequenceInfo &other, const SequenceDiffDdlOptions &options) const
{
    QString ddl;

    if(options.updateCurrVal && lastNumber!=other.lastNumber){
        ddl.append(other.generateDropDdl()).append(";\n");
        SequenceCreateDdlOptions createOptions;
        createOptions.setInitialValue=true;
        ddl.append(generateDdl(createOptions));
    }else{

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
            if(cacheSize==0){
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
        }
    }

    return ddl;
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
