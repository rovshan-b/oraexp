#ifndef PARTITIONINGINFO_H
#define PARTITIONINGINFO_H

#include <QList>
#include <QMetaType>
#include "enums.h"
#include "partitioninfo.h"

class TableCreatorPartitionsTab;

class PartitioningInfo
{
public:
    PartitioningInfo();

    int partitioningStrategy;

    OraExp::PartitionType type;
    QString columns;
    int specifiedBy;
    int partitionCount;
    QString interval;
    QString foreignKey;
    QString storeIn;

    QList<PartitionInfo> partitions;


    QString generateDdl(OraExp::PartitionPart configureFor) const;
    QString generatePartitionListDdl(OraExp::PartitionPart configureFor,
                                     const PartitioningInfo *subpartitionsInfo,
                                     const QString &parentPartitionName) const;

    QList< NameQueryPair > generateDiffDdl(const PartitioningInfo &other,
                                                    const QString &schemaName,
                                                    const QString &objectName,
                                                    OraExp::PartitionPart configureFor,
                                                    bool configureForIndex, const PartitioningInfo *subpartitionInfo, QStringList &appendedPartitionNames) const;
    QList<NameQueryPair> generateIntervalAndStoreInDiffDdl(const PartitioningInfo &other, const QString &schemaName, bool configureForIndex) const;
    QList< NameQueryPair > generatePartitionListDiffDdl(const PartitioningInfo &other,
                                                                                   const QString &fullTableName,
                                                                                   OraExp::PartitionPart configureFor,
                                                                                   bool configureForIndex,
                                                                                   const PartitioningInfo *subpartitionInfo, QStringList &appendedPartitionNames) const;

    int commitChanges(const QString &taskName, const PartitioningInfo &modifiedPInfo);

    void removeIncorrectRows();

    const PartitionInfo *findPartitionByName(const QString &partName) const;
    QList<const PartitionInfo*> findSubpartitionsByPartitionByName(const QString &partName) const;
};

#endif // PARTITIONINGINFO_H
