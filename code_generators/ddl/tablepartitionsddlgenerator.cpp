#include "tablepartitionsddlgenerator.h"
#include "util/strutil.h"

TablePartitionsDdlGenerator::TablePartitionsDdlGenerator()
{
}

//this method is used by Compare Schemas tool
QString TablePartitionsDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, bool configureForIndex)
{
    QString ddl;

    QList<NameQueryPair> propsDiffDdl=sourceTableInfo.partitioningInfo.partitionInfo.generateIntervalAndStoreInDiffDdl(targetTableInfo.partitioningInfo.partitionInfo,
                                                                                                                       targetTableInfo.generalInfo.getFullTableName(),
                                                                                                                       false);
    foreach(const NameQueryPair &pair, propsDiffDdl){
        addEOL(ddl);
        ddl.append(pair.second).append(";");
    }

    ddl=TablePartitionsDdlGenerator::generatePartitionsDiffDdl(sourceTableInfo, targetTableInfo, configureForIndex);

    if(!configureForIndex){
        QStringList dummyList;
        QList<NameQueryPair> subpTemplateDiffDdls=sourceTableInfo.partitioningInfo.subpartitionTemplateInfo.generateDiffDdl(targetTableInfo.partitioningInfo.subpartitionTemplateInfo,
                                                                                                                            targetTableInfo.generalInfo.schema,
                                                                                                                            targetTableInfo.generalInfo.tableName,
                                                                                                                            OraExp::PartitionPartSubpartitionTemplate, false,
                                                                                                                            0, dummyList);
        foreach(const NameQueryPair &pair, subpTemplateDiffDdls){
            addEOL(ddl);
            ddl.append(pair.second).append(";");
        }
    }

    return ddl;
}

QString TablePartitionsDdlGenerator::generatePartitionsDiffDdl(const TableInfo &sourceTableInfo,
                                                               const TableInfo &targetTableInfo,
                                                               bool configureForIndex)
{
    QString ddl;

    //generate drop statements
    const QList<PartitionInfo> targetPartitions = targetTableInfo.partitioningInfo.partitionInfo.partitions;
    foreach(const PartitionInfo &targetPInfo, targetPartitions){
        const PartitionInfo *sourcePInfo=sourceTableInfo.partitioningInfo.partitionInfo.findPartitionByName(targetPInfo.partitionName);
        if(sourcePInfo==0){ //not exists in source table, so must generate drop statement to remove it from target table as well
            addEOL(ddl);
            ddl.append(targetPInfo.generateDropDdl(targetTableInfo.generalInfo.getFullTableName(), OraExp::PartitionPartPartition, configureForIndex)).append(";");
        }
    }

    const QList<PartitionInfo> sourcePartitions = sourceTableInfo.partitioningInfo.partitionInfo.partitions;
    foreach(const PartitionInfo &sourcePInfo, sourcePartitions){
        const PartitionInfo *targetPInfo=targetTableInfo.partitioningInfo.partitionInfo.findPartitionByName(sourcePInfo.partitionName);
        if(targetPInfo==0){ //generate add statement
            addEOL(ddl);
            ddl.append(sourcePInfo.generateAppendDdl(targetTableInfo.generalInfo.getFullTableName(), sourceTableInfo.partitioningInfo.partitionInfo.type,
                                                     OraExp::PartitionPartPartition, configureForIndex, &sourceTableInfo.partitioningInfo.subpartitionInfo)).append(";");
        }else{ //generate alter statement
            const QList<NameQueryPair> &alterDdls = sourcePInfo.generateDiffDdl(*targetPInfo, targetTableInfo.generalInfo.getFullTableName(),
                                                                                OraExp::PartitionPartPartition, configureForIndex);
            foreach(const NameQueryPair &pair, alterDdls){
                addEOL(ddl);
                ddl.append(pair.second).append(";");
            }

            //generate diff for subpartitions
            if(!configureForIndex){
                TablePartitionsDdlGenerator::generateSubpartitionsDiffDdl(sourceTableInfo, targetTableInfo, sourcePInfo.partitionName, ddl);
            }
        }
    }

    return ddl;
}

void TablePartitionsDdlGenerator::generateSubpartitionsDiffDdl(const TableInfo &sourceTableInfo,
                                                                  const TableInfo &targetTableInfo,
                                                                  const QString &partitionName,
                                                                  QString &ddl)
{
    QList<const PartitionInfo*> sourceSubpartitionList = sourceTableInfo.partitioningInfo.subpartitionInfo.findSubpartitionsByPartitionByName(partitionName);
    QList<const PartitionInfo*> targetSubpartitionList = targetTableInfo.partitioningInfo.subpartitionInfo.findSubpartitionsByPartitionByName(partitionName);
    for(int i=0; i<sourceSubpartitionList.size(); ++i){
        if(i<targetSubpartitionList.size()){ //generate diff ddl
            const QList<NameQueryPair> &alterDdls=
                    sourceSubpartitionList.at(i)->generateDiffDdl(*targetSubpartitionList.at(i),
                                                          targetTableInfo.generalInfo.getFullTableName(),
                                                          OraExp::PartitionPartSubpartition, false);

            foreach(const NameQueryPair &pair, alterDdls){
                addEOL(ddl);
                ddl.append(pair.second).append(";");
            }

        }else{ //generate append ddl
            QString appendDdl=sourceSubpartitionList.at(i)->generateAppendDdl(targetTableInfo.generalInfo.getFullTableName(),
                                                            targetTableInfo.partitioningInfo.subpartitionInfo.type,
                                                                              OraExp::PartitionPartSubpartition, false, 0).append(";");
            addEOL(ddl);
            ddl.append(appendDdl);
        }
    }

    for(int i=sourceSubpartitionList.size(); i<targetSubpartitionList.size(); ++i){
        addEOL(ddl);
        ddl.append(targetSubpartitionList.at(i)->generateDropDdl(targetTableInfo.generalInfo.getFullTableName(),
                                                                 OraExp::PartitionPartSubpartition, false)).append(";");
    }
}
