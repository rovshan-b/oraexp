#include "partitioninginfo.h"
#include "util/dbutil.h"
#include "util/strutil.h"

PartitioningInfo::PartitioningInfo() : specifiedBy(1), partitionCount(-1)
{
}

QString PartitioningInfo::generateDdl(OraExp::PartitionPart configureFor) const
{
    QString ddl;

    QString colList;
    if(!columns.isEmpty() && type!=OraExp::PartitionTypeReference){
        colList.append(columns);
    }else if(!foreignKey.isEmpty() && type==OraExp::PartitionTypeReference){
        colList.append(foreignKey);
    }

    if(colList.isEmpty() && configureFor!=OraExp::PartitionPartSubpartitionTemplate){
        return ddl;
    }

    if(configureFor!=OraExp::PartitionPartSubpartitionTemplate){
        ddl.append("\n");
        if(configureFor==OraExp::PartitionPartSubpartition){ddl.append("SUB");}
        ddl.append("PARTITION BY ").append(DbUtil::getPartitionTypeName(type));
        ddl.append(" (");
        ddl.append(colList);
        ddl.append(")");
    }

    if(!interval.isEmpty() && type==OraExp::PartitionTypeRange){
        ddl.append("\nINTERVAL(").append(interval).append(")");
    }

    if(type==OraExp::PartitionTypeHash && specifiedBy==0)
    {
        if(partitionCount>0){
            ddl.append("\n");
            if(configureFor!=OraExp::PartitionPartPartition){ddl.append("SUB");}
            ddl.append("PARTITIONS ").append(QString::number(partitionCount));
        }
    }

    if(!storeIn.isEmpty() &&
            ((type==OraExp::PartitionTypeHash && specifiedBy==0) ||
                (type==OraExp::PartitionTypeRange && !interval.isEmpty()))){
        ddl.append("\nSTORE IN (").append(storeIn).append(")");
    }

    return ddl;
}

QString PartitioningInfo::generatePartitionListDdl(OraExp::PartitionPart configureFor,
                                                        const PartitioningInfo *subpartitionsInfo,
                                                        const QString &parentPartitionName) const
{
    QString ddl;

    int rowCount=partitions.size();

    QString parentPartitionNames;
    PartitionInfo pInfo;

    for(int i=0; i<rowCount; ++i){

        pInfo=partitions.at(i);

        if(configureFor==OraExp::PartitionPartSubpartition){
            parentPartitionNames=pInfo.partitionName;
            QStringList items=splitTrimmed(parentPartitionNames);

            if(!items.contains(parentPartitionName) && !items.isEmpty()){
                continue;
            }else if(!pInfo.subpartitionName.isEmpty() && items.size()!=1){
                pInfo.subpartitionName.prepend(QString("%1_").arg(parentPartitionName));
            }
        }

        if(pInfo.partitionPosition==-1){
            continue;
        }

        if(!ddl.isEmpty()){
            ddl.append(",");
        }

        ddl.append("\n\t");
        ddl.append(pInfo.generateDdl(type, configureFor));

        if(subpartitionsInfo!=0){
            QString subpartitionListDdl=subpartitionsInfo->generatePartitionListDdl(OraExp::PartitionPartSubpartition, 0, pInfo.partitionName);
            if(!subpartitionListDdl.isEmpty()){
                ddl.append(subpartitionListDdl);
            }
        }
    }

    if(ddl.size()>0){
        if(configureFor==OraExp::PartitionPartSubpartition){
            ddl.prepend("\n\t(");
            ddl.append("\n\t)");
        }else{
            ddl.prepend("\n(");
            ddl.append("\n)");
        }
    }

    return ddl;
}

QList< NameQueryPair > PartitioningInfo::generateDiffDdl(const PartitioningInfo &other,
                                                                  const QString &schemaName,
                                                                  const QString &objectName,
                                                                  OraExp::PartitionPart configureFor,
                                                                  bool configureForIndex,
                                                                  const PartitioningInfo *subpartitionInfo,
                                                                  QStringList &appendedPartitionNames) const
{
    QList< NameQueryPair > result;

    QString fullObjectName=QString("\"%1\".\"%2\"").arg(schemaName, objectName);
    QString alterPrefix=QString("ALTER %1 %2").arg(configureForIndex ? "INDEX" : "TABLE", fullObjectName);
    QString ddl;

    if(configureFor==OraExp::PartitionPartSubpartitionTemplate)
    {
        if((specifiedBy!=other.specifiedBy || storeIn!=other.storeIn || partitionCount!=other.partitionCount
            || (specifiedBy==1 && partitions!=other.partitions))){
            QString subpTemplateDdl=QString("%1 %2").arg(generateDdl(configureFor),
                                                      generatePartitionListDdl(configureFor, 0, "")).trimmed();
            ddl=QString("%1 SET SUBPARTITION TEMPLATE %2").arg(alterPrefix, subpTemplateDdl.isEmpty() ? "( )" : subpTemplateDdl);

            result.append(qMakePair(QString("table_subpartition_template_set"), ddl));
        }

        return result;
    }

    result.append(generateIntervalAndStoreInDiffDdl(other, fullObjectName, configureForIndex));

    result.append(generatePartitionListDiffDdl(other, fullObjectName, configureFor, configureForIndex, subpartitionInfo, appendedPartitionNames));

    return result;
}

QList< NameQueryPair > PartitioningInfo::generateIntervalAndStoreInDiffDdl(const PartitioningInfo &other,
                                                                           const QString &fullObjectName,
                                                                           bool configureForIndex) const
{
    QList< NameQueryPair > result;

    QString alterPrefix=QString("ALTER %1 %2").arg(configureForIndex ? "INDEX" : "TABLE", fullObjectName);
    QString ddl;

    if(interval!=other.interval){
        ddl=QString("%1 SET INTERVAL ( %2 )").arg(alterPrefix, interval);
        result.append(qMakePair(QString("table_partition_interval"), ddl));
    }

    if(storeIn!=other.storeIn){
        ddl=QString("%1 SET SET STORE IN ( %2 )").arg(alterPrefix, storeIn);
        result.append(qMakePair(QString("table_partition_store_in"), ddl));
    }

    return result;
}

QList< NameQueryPair > PartitioningInfo::generatePartitionListDiffDdl(const PartitioningInfo &other,
                                                                               const QString &fullTableName,
                                                                               OraExp::PartitionPart configureFor,
                                                                               bool configureForIndex,
                                                                               const PartitioningInfo *subpartitionInfo,
                                                                               QStringList &appendedPartitionNames) const
{
    QList< NameQueryPair > result;

    Q_ASSERT(partitions.size()>=other.partitions.size());

    int lowerPartCount=qMin(this->partitions.size(), other.partitions.size());
    for(int i=0; i<lowerPartCount; ++i){
        result.append(partitions.at(i).generateDiffDdl(other.partitions.at(i), fullTableName, configureFor, configureForIndex));
    }

    PartitionInfo info;
    QString taskName=QString("table_%1").arg(configureFor==OraExp::PartitionPartPartition ? "partition" : "subpartition");

    //generate add statements
    QString addDdl;
    for(int i=other.partitions.size(); i<partitions.size(); ++i){
        info=partitions.at(i);
        if(info.partitionPosition!=-1){
            if(configureFor==OraExp::PartitionPartSubpartition && appendedPartitionNames.contains(info.partitionName)){
                continue;
            }

            addDdl=info.generateAppendDdl(fullTableName, type, configureFor, configureForIndex, subpartitionInfo);
            if(configureFor==OraExp::PartitionPartPartition){
                appendedPartitionNames.append(info.partitionName);;
            }
            if(!addDdl.isEmpty()){
                result.append(qMakePair(QString("%1_add_%2").arg(taskName, QString::number(info.partitionPosition)), addDdl));
            }
        }
    }

    return result;
}

int PartitioningInfo::commitChanges(const QString &taskName, const PartitioningInfo &modifiedPInfo)
{
    if(taskName=="table_subpartition_template_set"){
        *this=modifiedPInfo;
        return -1;
    }else if(taskName=="table_partition_interval"){
        interval=modifiedPInfo.interval;
        return -1;
    }else if(taskName=="table_partition_store_in"){
        storeIn=modifiedPInfo.storeIn;
        return -1;
    }

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    int rowIx=rowNum-1;

    if(!taskName.startsWith("table_partition_add") && !taskName.startsWith("table_subpartition_add")){
        Q_ASSERT(partitions.size()>rowIx);
    }

    if(taskName.startsWith("table_partition_drop") || taskName.startsWith("table_subpartition_drop")){

        partitions[rowIx].dropped=true;

    }else if(taskName.startsWith("table_partition_rename")){

        partitions[rowIx].partitionName=modifiedPInfo.partitions[rowIx].partitionName;

    }else if(taskName.startsWith("table_subpartition_rename")){

        partitions[rowIx].subpartitionName=modifiedPInfo.partitions[rowIx].subpartitionName;

    }else if(taskName.startsWith("table_partition_add_values") ||
             taskName.startsWith("table_subpartition_add_values") ||
             taskName.startsWith("table_partition_drop_values") ||
             taskName.startsWith("table_subpartition_drop_values")){

        partitions[rowIx].highValue=modifiedPInfo.partitions[rowIx].highValue;

    }else if(taskName.startsWith("table_partition_add") || taskName.startsWith("table_subpartition_add")){

        partitions.append(modifiedPInfo.partitions.at(rowIx));

    }

    return rowIx;

}

void PartitioningInfo::removeIncorrectRows()
{
    int partitionCount=partitions.size();
    for(int i=partitionCount-1; i>=0; --i){
        if(partitions.at(i).partitionPosition==-1){
            partitions.removeAt(i);
        }
    }
}

//must be called only when current PartitioningInfo represents partition info
const PartitionInfo *PartitioningInfo::findPartitionByName(const QString &partName) const
{
    for(int i=0; i<partitions.size(); ++i){
        if(partitions.at(i).partitionName==partName){
            return &partitions.at(i);
        }
    }

    return 0;
}

//must be called only when current PartitioningInfo represents subpartition info
QList<const PartitionInfo*> PartitioningInfo::findSubpartitionsByPartitionByName(const QString &partName) const
{
    QList<const PartitionInfo*> results;

    for(int i=0; i<partitions.size(); ++i){
        if(partitions.at(i).partitionName==partName){
            results.append(&partitions[i]);
        }
    }

    return results;
}
