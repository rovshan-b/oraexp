#include "indexorganizedtableproperties.h"
#include "util/strutil.h"

IndexOrganizedTableProperties::IndexOrganizedTableProperties() :
    pctThreshold(-1),
    mappingTable(0),
    keyCompression(0),
    keyCompressionPrefixLength(-1),
    overflow(false),
    logging(0)
{
}

QString IndexOrganizedTableProperties::generateDdl() const
{
    QString ddl;

    if(pctThreshold!=-1){
        ddl.append(" PCTTHRESHOLD ").append(QString::number(pctThreshold));
    }

    ddl.append(optionText(mappingTable, " MAPPING TABLE", " NOMAPPING"));

    if(keyCompression==1){
        ddl.append(" COMPRESS ");
        if(keyCompressionPrefixLength!=-1){
            ddl.append(QString::number(keyCompressionPrefixLength));
        }
    }else if(keyCompression==2){
        ddl.append(" NOCOMPRESS");
    }

    if(overflow){
        ddl.append(" OVERFLOW");

        if(!including.isEmpty()){
            ddl.append(" INCLUDING \"").append(including).append("\" ");
        }

        QString storageParamsDdl=storageParams.generateDdl();
        if(!storageParamsDdl.isEmpty()){
            ddl.append(" ").append(storageParamsDdl);
        }

        ddl.append(optionText(logging, " LOGGING ", " NOLOGGING "));
    }

    return ddl;
}

QString IndexOrganizedTableProperties::generateDiffDdl(const IndexOrganizedTableProperties &other) const
{
    QString ddl;

    if(pctThreshold!=other.pctThreshold){
        ddl.append(" PCTTHRESHOLD ").append(QString::number(pctThreshold));
    }

    QString overflowDdl;
    if(overflow){

        if(!including.isEmpty() && including!=other.including){
            overflowDdl.append(" INCLUDING \"").append(including).append("\" ");
        }

        QString storageParamsDdl=storageParams.generateDiffDdl(other.storageParams);
        if(!storageParamsDdl.isEmpty()){
            overflowDdl.append(storageParamsDdl);
        }

        if(logging!=other.logging){
            overflowDdl.append(optionText(logging, " LOGGING", " NOLOGGING"));
        }

        if(!other.overflow){
            overflowDdl.prepend(" ADD OVERFLOW");
        }else if(!overflowDdl.isEmpty()){
            overflowDdl.prepend(" OVERFLOW");
        }
    }

    if(!overflowDdl.isEmpty()){
        ddl.append(overflowDdl);
    }

    return ddl;
}

IndexOrganizedTableProperties IndexOrganizedTableProperties::fromFetchResult(const FetchResult &result)
{
    IndexOrganizedTableProperties p;

    p.pctThreshold=result.colValue("PCT_THRESHOLD", -1);
    p.mappingTable=result.colValue("HAS_MAPPING_TABLE", -1) == 1 ? 1 : 2;
    int prefixLength=result.colValue("PREFIX_LENGTH", -1);
    if(prefixLength>0){
        p.keyCompression=1;
        p.keyCompressionPrefixLength=prefixLength;
    }else{
        p.keyCompression=2;
    }
    p.overflow=!result.colValue("OVERFLOW_TABLE_NAME").isEmpty();

    if(p.overflow){
        p.including=result.colValue("INCLUDE_COL_NAME");
        p.logging=result.colValue("OVERFLOW_LOGGING")=="YES" ? 1 : 2;
    }

    return p;
}
