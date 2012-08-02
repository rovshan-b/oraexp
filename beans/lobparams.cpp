#include "lobparams.h"
#include "util/strutil.h"

LobParams::LobParams() :
    cache(0),
    logging(0),
    chunk(-1),
    pctVersion(-1),
    freePools(-1),
    enableStorageInRow(false),
    enableRetention(false),
    isEmpty(false)
{
}

bool LobParams::operator==(const LobParams &other) const
{
    return segment==other.segment &&
           tablespaceName==other.tablespaceName &&
            cache==other.cache &&
            logging==other.logging &&
            chunk==other.chunk &&
            pctVersion==other.pctVersion &&
            freePools==other.freePools &&
            enableStorageInRow==other.enableStorageInRow &&
            enableRetention==other.enableRetention &&
            isEmpty==other.isEmpty &&
            (storageParams.isEmpty || storageParams==other.storageParams);

}

QString LobParams::generateDdl(bool storage, bool tablespaceOnly) const
{
    QString ddl;

    QString lobParamsDdl;

    if(!tablespaceName.isEmpty() ||
            cache!=0 ||
            logging!=0 ||
            chunk!=-1 ||
            pctVersion!=-1 ||
            freePools!=-1 ||
            enableStorageInRow!=false ||
            enableRetention!=false){

        if(!tablespaceName.isEmpty() && storage){
            lobParamsDdl.append("TABLESPACE \"").append(tablespaceName.trimmed().toUpper()).append("\" ");
        }

        lobParamsDdl.append(optionText(cache, "CACHE ", "NOCACHE ", "CACHE READS "));

        if(cache==2 /*NOCACHE*/ || cache==3 /*CACHE READS*/){
            lobParamsDdl.append(optionText(logging, "LOGGING ", "NOLOGGING "));
        }

        if(chunk!=-1){
            lobParamsDdl.append("CHUNK ").append(QString::number(chunk)).append(" ");
        }

        if(pctVersion!=-1){
            lobParamsDdl.append("PCTVERSION ").append(QString::number(pctVersion)).append(" ");
        }

        if(freePools!=-1){
            lobParamsDdl.append("FREEPOOLS ").append(QString::number(freePools)).append(" ");
        }

        if(enableStorageInRow){
            lobParamsDdl.append("ENABLE STORAGE IN ROW ");
        }

        if(enableRetention){
            lobParamsDdl.append("RETENTION ");
        }
    }

    if(!segment.isEmpty()){
        ddl.append(segment.trimmed().toUpper()).append(" ");
    }

    QString storageParamsDdl=(storage && !tablespaceOnly) ? storageParams.generateDdl(true) : "";

    if(!lobParamsDdl.isEmpty() || !storageParamsDdl.isEmpty()){
        ddl.append("( ");

        if(!lobParamsDdl.isEmpty()){
            ddl.append(lobParamsDdl);
        }

        if(!storageParamsDdl.isEmpty()){
            ddl.append("\n");
            ddl.append(storageParamsDdl);
        }

        ddl.append(")");
    }

    return ddl;
}

QString LobParams::generateDiffDdl(const LobParams &other) const
{
    QString ddl;
    QString lobParamsDdl;

    if(cache!=other.cache){
        lobParamsDdl.append(optionText(cache, " CACHE", " NOCACHE", " CACHE READS"));
    }

    if(logging!=other.logging){
        lobParamsDdl.append(optionText(logging, " LOGGING", " NOLOGGING"));
    }

    if(pctVersion!=other.pctVersion && pctVersion!=-1){
        lobParamsDdl.append(" PCTVERSION ").append(QString::number(pctVersion)).append(" ");
    }

    if(freePools!=other.freePools){
        lobParamsDdl.append(" FREEPOOLS ").append(QString::number(freePools==-1 ? 1 : freePools)).append(" ");
    }

    if(enableRetention!=other.enableRetention){
        if(enableRetention){
            lobParamsDdl.append(" RETENTION");
        }else{
            lobParamsDdl.append(" RETENTION NONE");
        }
    }

    QString storageParamsDdl=storageParams.generateDiffDdl(other.storageParams);

    if(!lobParamsDdl.isEmpty() || !storageParamsDdl.isEmpty()){
        ddl.append("( ");

        if(!lobParamsDdl.isEmpty()){
            ddl.append(lobParamsDdl);
        }

        if(!storageParamsDdl.isEmpty()){
            ddl.append("\n");
            ddl.append(storageParamsDdl);
        }

        ddl.append(")");
    }

    return ddl.trimmed();
}

LobParams LobParams::fromFetchResult(const FetchResult &result)
{
    LobParams p;

    p.segment=result.colValue("SEGMENT_NAME");
    p.tablespaceName=result.colValue("TABLESPACE_NAME");
    QString cache=result.colValue("CACHE");
    if(cache=="YES"){
        p.cache=1;
    }else if(cache=="NO"){
        p.cache=2;
    }else if(cache=="CACHEREADS"){
        p.cache=3;
    }
    p.logging=result.colValue("LOGGING")=="YES" ? 1 : 2;
    p.chunk=result.colValue("CHUNK", -1);
    p.pctVersion=result.colValue("PCTVERSION", -1);
    p.freePools=result.colValue("FREEPOOLS", -1);
    p.enableStorageInRow=result.colValue("IN_ROW")=="YES";
    QString retention=result.colValue("RETENTION");
    p.enableRetention=!retention.isEmpty() && retention!="0";

    if(p.pctVersion && p.enableRetention){
        p.pctVersion=-1;
    }

    p.storageParams=StorageParams::fromFetchResult(result);

    return p;

}
