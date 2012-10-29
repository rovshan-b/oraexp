#include "storageparams.h"
#include "constants.h"
#include "util/dbutil.h"

StorageParams::StorageParams() :
    pctFree(-1), pctUsed(-1),
    initTrans(-1), maxTrans(-1),
    initialExtent(-1), initialExtentUnit(OraExp::Bytes),
    nextExtent(-1), nextExtentUnit(OraExp::Bytes),
    pctIncrease(-1), minExtents(-1), maxExtents(-1),
    unlimitedMaxExtents(false),
    maxSize(-1), maxSizeUnit(OraExp::Bytes), unlimitedMaxSize(false),
    freeLists(-1), freeListGroups(-1), isEmpty(false)
{
}

bool StorageParams::operator==(const StorageParams &other) const
{
    return tablespaceName==other.tablespaceName &&
            pctFree==other.pctFree &&
            pctUsed==other.pctUsed &&
            initTrans==other.initTrans &&
            maxTrans==other.maxTrans &&
            initialExtent==other.initialExtent &&
            initialExtentUnit==other.initialExtentUnit &&
            nextExtent==other.nextExtent &&
            nextExtentUnit==other.nextExtentUnit &&
            pctIncrease==other.pctIncrease &&
            minExtents==other.minExtents &&
            maxExtents==other.maxExtents &&
            unlimitedMaxExtents==other.unlimitedMaxExtents &&
            maxSize==other.maxSize &&
            maxSizeUnit==other.maxSizeUnit &&
            unlimitedMaxSize==other.unlimitedMaxSize &&
            freeLists==other.freeLists &&
            freeListGroups==other.freeListGroups &&
            bufferPool==other.bufferPool &&
            isEmpty==other.isEmpty;
}

QString StorageParams::generateDdl(bool configureForLob, bool tablespaceOnly) const
{
    QString ddl;

    if(!tablespaceName.isEmpty() && !configureForLob){
        ddl.append("TABLESPACE \"").append(tablespaceName).append("\" ");
    }

    if(tablespaceOnly){ //return if we need to generate only tablespace name
        return ddl;
    }

    if(pctFree>0){
        ddl.append("PCTFREE ").append(QString::number(pctFree)).append(" ");
    }

    if(pctUsed>0){
        ddl.append("PCTUSED ").append(QString::number(pctUsed)).append(" ");
    }

    if(initTrans>0){
        ddl.append("INITRANS ").append(QString::number(initTrans)).append(" ");
    }

    if(maxTrans>0){
        ddl.append("MAXTRANS ").append(QString::number(maxTrans)).append(" ");
    }

    if(initialExtent>0 || nextExtent>0 ||
            pctIncrease>0 || minExtents>0 ||
            maxExtents>0 || unlimitedMaxExtents ||
            maxSize>0 || unlimitedMaxSize ||
            freeLists>0 || freeListGroups>0 || !bufferPool.isEmpty()){
        ddl.append("STORAGE ( ");

        if(initialExtent>0){
            ddl.append("INITIAL ").append(QString::number(initialExtent)).
                append(DbUtil::getSizeUnitText(initialExtentUnit)).append(" ");
        }

        if(nextExtent>0){
            ddl.append("NEXT ").append(QString::number(nextExtent)).
                append(DbUtil::getSizeUnitText(nextExtentUnit)).append(" ");
        }

        if(pctIncrease>0){
            ddl.append("PCTINCREASE ").append(QString::number(pctIncrease)).append(" ");
        }

        if(minExtents>0){
            ddl.append("MINEXTENTS ").append(QString::number(minExtents)).append(" ");
        }

        if(maxExtents>0 || unlimitedMaxExtents){
            ddl.append("MAXEXTENTS ").append(unlimitedMaxExtents ? "UNLIMITED" : QString::number(maxExtents)).append(" ");
        }

        if(maxSize>0 || unlimitedMaxSize){
            ddl.append("MAXSIZE ").append(unlimitedMaxSize ?
                                              "UNLIMITED" :
                                              QString::number(maxSize).append(DbUtil::getSizeUnitText(maxSizeUnit))).append(" ");
        }

        if(freeLists>0){
            ddl.append("FREELISTS ").append(QString::number(freeLists)).append(" ");
        }

        if(freeListGroups>0){
            ddl.append("FREELIST GROUPS ").append(QString::number(freeListGroups)).append(" ");
        }

        if(!bufferPool.isEmpty()){
            ddl.append("BUFFER_POOL ").append(bufferPool).append(" ");
        }

        ddl.append(")");
    }

    return ddl;
}

QString StorageParams::generateDiffDdl(const StorageParams &other, bool configureForIndex) const
{
    QString ddl;

    if(pctFree!=other.pctFree && !configureForIndex){
        ddl.append(" PCTFREE ").append(QString::number(pctFree));
    }

    if(pctUsed!=other.pctUsed && pctUsed!=-1 && !configureForIndex){
        ddl.append(" PCTUSED ").append(QString::number(pctUsed));
    }

    if(initTrans!=other.initTrans){
        ddl.append(" INITRANS ").append(QString::number(initTrans));
    }

    if(maxTrans!=other.maxTrans){
        ddl.append(" MAXTRANS ").append(QString::number(maxTrans));
    }

    QString storageDdl;

    if(this->maxSize!=other.maxSize ||
            this->unlimitedMaxSize!=other.unlimitedMaxSize){
        storageDdl.append(" MAXSIZE ").append(unlimitedMaxSize ?
                                          "UNLIMITED" :
                                          QString::number(maxSize).append(DbUtil::getSizeUnitText(maxSizeUnit)));
    }

    if(freeLists!=other.freeLists && freeLists!=-1){
        storageDdl.append(" FREELISTS ").append(QString::number(freeLists));
    }

    if(bufferPool!=other.bufferPool
            && !(bufferPool.isEmpty() && other.bufferPool=="DEFAULT")){
        storageDdl.append(" BUFFER_POOL ").append(bufferPool);
    }

    if(!storageDdl.isEmpty()){
        ddl.append(" STORAGE (").append(storageDdl).append(" )");
    }

    return ddl;
}

StorageParams StorageParams::fromFetchResult(const FetchResult &result)
{
    StorageParams p;

    p.tablespaceName=result.colValue("TABLESPACE_NAME");
    p.pctFree=result.colValue("PCT_FREE", -1);
    p.pctUsed=result.colValue("PCT_USED", -1);
    p.initTrans=result.colValue("INI_TRANS", -1);
    p.maxTrans=result.colValue("MAX_TRANS", -1);

    p.initialExtent=result.colValue("INITIAL_EXTENT", -1);
    p.nextExtent=result.colValue("NEXT_EXTENT", -1);
    p.pctIncrease=result.colValue("PCT_INCREASE", -1);
    p.minExtents=result.colValue("MIN_EXTENTS", -1);

    p.maxExtents=result.colValue("MAX_EXTENTS", -1);
    if(p.maxExtents==ORACLE_MAX_EXTENT_SIZE){
        p.maxExtents=-1;
        p.unlimitedMaxExtents=true;
    }

    p.maxSize=result.colValue("MAX_SIZE", -1);
    if(p.maxSize==ORACLE_MAX_EXTENT_SIZE){
        p.maxSize=-1;
        p.unlimitedMaxSize=true;
    }

    p.freeLists=result.colValue("FREELISTS", -1);
    p.freeListGroups=result.colValue("FREELIST_GROUPS", -1);
    p.bufferPool=result.colValue("BUFFER_POOL");

    return p;

}
