#include "indexinfo.h"
#include "util/dbutil.h"

IndexInfo::IndexInfo() : indexId(-1),
                         prefixLength(-1), parallelDegree(-1),
                         partitioning(0),
                         generated(false),
                         markedForDeletion(false),
                         dropped(false)
{
}

bool IndexInfo::operator==(const IndexInfo &other) const
{
    return name==other.name &&
           parallel==other.parallel &&
           parallelDegree==other.parallelDegree &&
           compress==other.compress &&
           prefixLength==other.prefixLength &&
           !needsRecreation(other);
}

IndexInfo IndexInfo::fromFetchResult(const FetchResult &result)
{
    IndexInfo i;

    i.owner=result.colValue("OWNER");
    i.name=result.colValue("INDEX_NAME");

    QString indexType=result.colValue("INDEX_TYPE");
    if(indexType=="BITMAP"){
        i.type=OraExp::BitmapIndex;
    }
    //else if(indexType=="FUNCTION-BASED NORMAL"){
    //    i.type=OraExp::FunctionBasedIndex;
    //}
    else if(result.colValue("UNIQUENESS")=="UNIQUE"){
        i.type=OraExp::UniqueIndex;
    }
    else{
        i.type=OraExp::NormalIndex;
    }

    i.columns=result.colValue("COLUMNS");
    i.compress=result.colValue("COMPRESSION")=="ENABLED";
    i.prefixLength=result.colValue("PREFIX_LENGTH", -1);

    QString degree=result.colValue("DEGREE").trimmed();
    i.parallel=(degree=="1" ? 2 : 1);
    i.parallelDegree=((degree=="1" || degree=="DEFAULT") ? -1 : degree.toInt());
    i.reverse=(indexType=="NORMAL/REV");
    i.storageParams=StorageParams::fromFetchResult(result);
    i.generated=result.colValue("GENERATED")=="Y";
    if(result.colValue("PARTITIONED")=="YES"){
        i.partitioning=result.colValue("LOCALITY")=="LOCAL" ? 1 : 2;
    }

    return i;

}

QString IndexInfo::generateDdl(const QString &fullTableName, OraExp::TableType tableType, bool storage, bool tablespaceOnly) const
{
    QString ddl;

    ddl.append("CREATE ");
    if(type!=OraExp::NormalIndex){
        ddl.append(DbUtil::getIndexTypeName(type, false).toUpper()).append(" ");
    }

    ddl.append("INDEX \"").append(owner).append("\".\"").append(name).append("\" ON ").
    append(fullTableName).append(" (").append(columns).append(") ");

    if(compress){
        ddl.append("COMPRESS ");
        if(prefixLength!=-1){
            ddl.append(QString::number(prefixLength)).append(" ");
        }
    }

    if(reverse && type!=OraExp::BitmapIndex){
        ddl.append("REVERSE ");
    }

    if(parallel==1){ //enabled
        ddl.append("PARALLEL ");
        if(parallelDegree!=-1){
            ddl.append(QString::number(parallelDegree)).append(" ");
        }
    }else if(parallel==2){ //disabled
        ddl.append("NOPARALLEL ");
    }

    if(storage){
        ddl.append(storageParams.generateDdl(false, tablespaceOnly));
    }

    if(partitioning==1){
        ddl.append(" LOCAL");
    }else if(partitioning==2){
        ddl.append(" GLOBAL ").append(partInfo.generateDdl(tableType));
    }

    return ddl;
}

QString IndexInfo::generateDropDdl(const QString &schemaName) const
{
    return QString("DROP INDEX \"%1\".\"%2\"").arg(schemaName, name);
}

QList< NameQueryPair > IndexInfo::generateDiffDdl(const IndexInfo &other,
                                                                const QString &schemaName,
                                                                const QString &tableName,
                                                                OraExp::TableType tableType,
                                                                bool includePartitioningDiff /*this argument will be true when function
                                                                is called from schema comparer tool*/) const
{
    QString fullTableName=QString("\"%1\".\"%2\"").arg(schemaName).arg(tableName);

    QList< NameQueryPair > result;

    QString ddl;

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(schemaName);
        result.append(qMakePair(QString("drop_index_%1").arg(indexId), ddl));

    }else if(needsRecreation(other) ||
             (includePartitioningDiff==false && partitioning==2 &&
              partInfo.partitionInfo.partitions.size()!=other.partInfo.partitionInfo.partitions.size())){
        QString ddl=other.generateDropDdl(schemaName);

        result.append(qMakePair(QString("drop_index_for_recreation_%1").arg(indexId), ddl));
        result.append(qMakePair(QString("create_index_after_dropping_%1").arg(indexId), generateDdl(fullTableName, tableType).trimmed()));
    }else {

        if(name!=other.name && !generated){
            ddl=QString("ALTER INDEX \"%1\".\"%2\" RENAME TO \"%3\"").arg(schemaName, other.name, name);
            result.append(qMakePair(QString("rename_index_%1").arg(indexId), ddl));
            ddl="";
        }

        if(parallel != other.parallel || parallelDegree != other.parallelDegree){
            if(parallel==1){ //enabled
                ddl.append(" PARALLEL");
                if(parallelDegree!=-1){
                    ddl.append(" ").append(QString::number(parallelDegree));
                }
            }else if(parallel==2){ //disabled
                ddl.append(" NOPARALLEL");
            }
        }

        if(compress != other.compress || prefixLength != other.prefixLength){
            if(compress){
                ddl.append(" COMPRESS");
                if(prefixLength!=-1){
                    ddl.append(" ").append(QString::number(prefixLength));
                }
            }else{
                ddl.append(" NOCOMPRESS");
            }
        }

        ddl.append(storageParams.generateDiffDdl(other.storageParams, true));

        if(!ddl.isEmpty()){
            ddl.prepend(QString("ALTER INDEX \"%1\".\"%2\"").arg(schemaName, other.name));
            result.append(qMakePair(QString("alter_index_%1").arg(indexId), ddl));
        }

        if(!partInfo.isEmpty || !other.partInfo.isEmpty){
            if(includePartitioningDiff || (partInfo.partitionInfo.partitions.size()==other.partInfo.partitionInfo.partitions.size())){
                QList< NameQueryPair > partitioningDiffDdls=partInfo.generateAlterDdl(other.partInfo, schemaName, name, true);
                foreach(NameQueryPair pair, partitioningDiffDdls){
                    result.append(qMakePair(QString("%1_%2").arg(pair.first, QString::number(indexId)), pair.second));
                }
            }
        }

    }

    return result;
}

bool IndexInfo::needsRecreation(const IndexInfo &other) const
{
    bool result = (owner!=other.owner ||
            type!=other.type ||
            columns!=other.columns ||
            reverse!=other.reverse ||
            partitioning!=other.partitioning
            );

    return result;
}

