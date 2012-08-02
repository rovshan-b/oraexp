#ifndef TABLEPARTITIONINGINFO_H
#define TABLEPARTITIONINGINFO_H

#include "partitioninginfo.h"
#include <QMetaType>

class TablePartitioningInfo
{
public:
    TablePartitioningInfo();

    PartitioningInfo partitionInfo;
    PartitioningInfo subpartitionInfo;
    PartitioningInfo subpartitionTemplateInfo;

    bool isEmpty;

    void setPartitionsTablespace(const QString &partitionsTablespace);

    QString generateDdl(OraExp::TableType tableType) const;
    QList< NameQueryPair > generateAlterDdl(const TablePartitioningInfo &other,
                                                     const QString &schemaName,
                                                     const QString &objectName,
                                                     bool configureForIndex) const;

    static bool canHaveSubpartitions(int partitioningStrategy, OraExp::TableType tableType, OraExp::PartitionType partitionType);
    static bool canHaveSubpartitionTemplate();
};

Q_DECLARE_METATYPE(TablePartitioningInfo)

#endif // TABLEPARTITIONINGINFO_H
