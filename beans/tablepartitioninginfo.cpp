#include "tablepartitioninginfo.h"
#include "util/strutil.h"

TablePartitioningInfo::TablePartitioningInfo() : isEmpty(true)
{
}

void TablePartitioningInfo::setPartitionsTablespace(const QString &partitionsTablespace)
{
    for(int i=0; i<partitionInfo.partitions.size(); ++i){
        partitionInfo.partitions[i].tablespaceName=partitionsTablespace;
    }
    for(int i=0; i<subpartitionInfo.partitions.size(); ++i){
        subpartitionInfo.partitions[i].tablespaceName=partitionsTablespace;
    }
    for(int i=0; i<subpartitionTemplateInfo.partitions.size(); ++i){
        subpartitionTemplateInfo.partitions[i].tablespaceName=partitionsTablespace;
    }
}

QString TablePartitioningInfo::generateDdl(OraExp::TableType tableType) const
{
    QString ddl;

    ddl.append(partitionInfo.generateDdl(OraExp::PartitionPartPartition));

    if(ddl.isEmpty()){
        return ddl;
    }

    bool subpartitions=TablePartitioningInfo::canHaveSubpartitions(partitionInfo.partitioningStrategy, tableType, partitionInfo.type);

    if(subpartitions){
        ddl.append(subpartitionInfo.generateDdl(OraExp::PartitionPartSubpartition));

        bool subpartitionTemplate=TablePartitioningInfo::canHaveSubpartitionTemplate();
        if(subpartitionTemplate){
            QString subPTemplate;
            subPTemplate.append(subpartitionTemplateInfo.generateDdl(OraExp::PartitionPartSubpartitionTemplate));
            subPTemplate.append(subpartitionTemplateInfo.generatePartitionListDdl(OraExp::PartitionPartSubpartitionTemplate, 0, ""));
            if(!subPTemplate.isEmpty()){
                ddl.append("\nSUBPARTITION TEMPLATE");
                ddl.append(subPTemplate);
            }
        }
    }

    ddl.append(partitionInfo.generatePartitionListDdl(OraExp::PartitionPartPartition, subpartitions ? &subpartitionInfo : 0, ""));

    return ddl;
}

QList< NameQueryPair > TablePartitioningInfo::generateAlterDdl(const TablePartitioningInfo &other,
                                                                        const QString &schemaName,
                                                                        const QString &objectName,
                                                                        bool configureForIndex) const
{
    QList< NameQueryPair > result;

    if(isEmpty || other.isEmpty){
        Q_ASSERT("Partitioning info is empty for source or target bean");
        return result;
    }

    QStringList appendedPartitionNames;
    result.append(partitionInfo.generateDiffDdl(other.partitionInfo, schemaName, objectName, OraExp::PartitionPartPartition,
                                                configureForIndex, &subpartitionInfo, appendedPartitionNames));

    if(other.partitionInfo.partitioningStrategy==1 && !configureForIndex){
        result.append(subpartitionInfo.generateDiffDdl(other.subpartitionInfo, schemaName, objectName,
                                                       OraExp::PartitionPartSubpartition, false, 0, appendedPartitionNames));
        result.append(subpartitionTemplateInfo.generateDiffDdl(other.subpartitionTemplateInfo, schemaName, objectName,
                                                               OraExp::PartitionPartSubpartitionTemplate, false, 0, appendedPartitionNames));
    }

    return result;
}

bool TablePartitioningInfo::canHaveSubpartitions(int partitioningStrategy, OraExp::TableType tableType, OraExp::PartitionType partitionType)
{
    return  partitioningStrategy==1 &&
            (!(partitionType==OraExp::PartitionTypeHash || partitionType==OraExp::PartitionTypeReference)) &&
            tableType!=OraExp::TableTypeIndexOrganized;
}

bool TablePartitioningInfo::canHaveSubpartitionTemplate()
{
    return true;
}
