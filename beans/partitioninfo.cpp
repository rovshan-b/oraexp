#include "partitioninfo.h"
#include "util/strutil.h"
#include "partitioninginfo.h"

PartitionInfo::PartitionInfo() : markedForDeletion(false), dropped(false)
{
}

bool PartitionInfo::operator==(const PartitionInfo &other) const
{
    return (partitionName==other.partitionName &&
            subpartitionName==other.subpartitionName &&
            highValue==other.highValue &&
            //partitionPosition==other.partitionPosition &&
            tablespaceName==other.tablespaceName &&
            markedForDeletion==other.markedForDeletion);
}

PartitionInfo PartitionInfo::fromFetchResult(const FetchResult &result)
{
    PartitionInfo i;

    i.partitionName=result.colValue("PARTITION_NAME");
    i.subpartitionName=result.colValue("SUBPARTITION_NAME");
    i.highValue=result.colValue("HIGH_VALUE");
    i.partitionPosition=result.colValue("PARTITION_POSITION", -1);
    //i.subpartitionPosition=result.colValue("SUBPARTITION_POSITION", -1);
    i.tablespaceName=result.colValue("TABLESPACE_NAME");

    return i;
}

QString PartitionInfo::generateDropDdl(const QString &fullTableName, OraExp::PartitionPart configureFor, bool configureForIndex) const
{
    QString ddl;

    QString configureForName=(configureFor==OraExp::PartitionPartPartition ? "PARTITION" : "SUBPARTITION");
    ddl=QString("ALTER %1 %2 DROP ").arg(configureForIndex ? "INDEX" : "TABLE", fullTableName);
    ddl.append(configureForName).append(" ").append(configureFor==OraExp::PartitionPartPartition ? partitionName : subpartitionName);

    return ddl;
}

QString PartitionInfo::generateAppendDdl(const QString &fullTableName,
                                         OraExp::PartitionType pType,
                                         OraExp::PartitionPart configureFor,
                                         bool configureForIndex,
                                         const PartitioningInfo *subpartitionInfo) const
{
    QString ddl;

    if(configureFor==OraExp::PartitionPartPartition){
        ddl=QString("ALTER %1 %2 ADD ").arg(configureForIndex ? "INDEX" : "TABLE", fullTableName);
        ddl.append(generateDdl(pType, configureFor));

        if(subpartitionInfo!=0){
            QString subpartitionsDdl=subpartitionInfo->generatePartitionListDdl(OraExp::PartitionPartSubpartition, 0, partitionName);
            if(!subpartitionsDdl.isEmpty()){
                ddl.append(subpartitionsDdl);
            }
        }
    }else{
        if(partitionName.isEmpty() || partitionName.contains(',')){
            return ddl;
        }

        ddl=QString("ALTER %1 %2 MODIFY PARTITION \"%3\" ADD ").arg(configureForIndex ? "INDEX" : "TABLE", fullTableName, partitionName);
        ddl.append(generateDdl(pType, configureFor));
    }

    return ddl;
}

QString PartitionInfo::generateDdl(OraExp::PartitionType pType, OraExp::PartitionPart configureFor) const
{
    QString ddl;

    if(configureFor!=OraExp::PartitionPartPartition){ddl.append("\tSUB");}
    ddl.append("PARTITION");

    if(configureFor==OraExp::PartitionPartPartition && !partitionName.isEmpty()){
        ddl.append(" \"").append(partitionName).append("\"");
    }else if(configureFor!=OraExp::PartitionPartPartition && !subpartitionName.isEmpty()){
        ddl.append(" \"").append(subpartitionName).append("\"");
    }

    if(pType==OraExp::PartitionTypeRange){
        ddl.append(" VALUES LESS THAN (").append(highValue).append(")");
    }else if(pType==OraExp::PartitionTypeList){
        ddl.append(" VALUES (").append(highValue).append(")");
    }

    if(!tablespaceName.isEmpty()){
        ddl.append(" TABLESPACE ").append(tablespaceName);
    }

    return ddl;
}

QList< NameQueryPair > PartitionInfo::generateDiffDdl(const PartitionInfo &other,
                                                               const QString &fullTableName,
                                                               OraExp::PartitionPart configureFor,
                                                               bool configureForIndex) const
{
    QList< NameQueryPair > result;

    if(other.dropped){
        return result;
    }

    QString ddl;
    QString alterPrefix=QString("ALTER %1 %2").arg(configureForIndex ? "INDEX" : "TABLE", fullTableName);

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(fullTableName, configureFor, configureForIndex);

        QString configureForName=(configureFor==OraExp::PartitionPartPartition ? "PARTITION" : "SUBPARTITION");
        result.append(qMakePair(QString("table_%1_drop_%2").arg(configureForName.toLower(), QString::number(partitionPosition)), ddl));

        return result;
    }

    if(configureFor==OraExp::PartitionPartPartition && partitionName!=other.partitionName){
        ddl.append(alterPrefix).append(" RENAME PARTITION \"").append(other.partitionName).append("\" TO \"").append(partitionName).append("\"");

        result.append(qMakePair(QString("table_partition_rename_%1").arg(partitionPosition), ddl));
    }else if(configureFor==OraExp::PartitionPartSubpartition && subpartitionName!=other.subpartitionName){
        ddl.append(alterPrefix).append(" RENAME SUBPARTITION \"").append(other.subpartitionName).append("\" TO \"").append(subpartitionName).append("\"");

        result.append(qMakePair(QString("table_subpartition_rename_%1").arg(partitionPosition), ddl));
    }

    if(highValue!=other.highValue){
        QString modifyPrefix=alterPrefix;
        modifyPrefix.append(" MODIFY ");
        if(configureFor==OraExp::PartitionPartSubpartition){
            modifyPrefix.append("SUB");
        }
        modifyPrefix.append("PARTITION \"").append(configureFor==OraExp::PartitionPartSubpartition ? subpartitionName : partitionName).append("\"");

        QString newListValues=getNewListValues(other.highValue, highValue);
        QString deletedListValues=getDroppedListValues(other.highValue, highValue);

        QString taskPrefix=configureFor==OraExp::PartitionPartSubpartition ? "table_subpartition" : "table_partition";
        if(!newListValues.isEmpty()){
            ddl=QString("%1 ADD VALUES ( %2 )").arg(modifyPrefix, newListValues);

            result.append(qMakePair(QString("%1_add_values_%2").arg(taskPrefix, QString::number(partitionPosition)), ddl));
        }

        if(!deletedListValues.isEmpty()){
            ddl=QString("%1 DROP VALUES ( %2 )").arg(modifyPrefix, deletedListValues);

            result.append(qMakePair(QString("%1_drop_values_%2").arg(taskPrefix, QString::number(partitionPosition)), ddl));
        }
    }

    return result;
}

QString PartitionInfo::getNewListValues(const QString &originalValues, const QString &newValues) const
{
    QStringList result;

    QStringList origList = splitTrimmed(originalValues, false);
    QStringList newList = splitTrimmed(newValues, false);

    for(int i=0; i<newList.size(); ++i){
        if(!origList.contains(newList.at(i))){
            result.append(newList.at(i));
        }
    }

    return result.join(",");
}

QString PartitionInfo::getDroppedListValues(const QString &originalValues, const QString &newValues) const
{
    QStringList result;

    QStringList origList = splitTrimmed(originalValues, false);
    QStringList newList = splitTrimmed(newValues, false);

    for(int i=0; i<origList.size(); ++i){
        if(!newList.contains(origList.at(i))){
            result.append(origList.at(i));
        }
    }

    return result.join(",");
}
