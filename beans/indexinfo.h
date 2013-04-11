#ifndef INDEXINFO_H
#define INDEXINFO_H

#include "enums.h"
#include "storageparams.h"
#include "defines.h"
#include "tablepartitioninginfo.h"

class IndexInfo
{
public:
    IndexInfo();

    bool operator==(const IndexInfo &other) const;

    int indexId;
    QString owner;
    QString name;
    OraExp::IndexType type;
    QString columns;
    bool compress;
    int prefixLength;
    int parallel;
    int parallelDegree;
    bool reverse;
    StorageParams storageParams;
    int partitioning;
    TablePartitioningInfo partInfo;
    bool generated;

    bool markedForDeletion;
    bool dropped;

    static IndexInfo fromFetchResult(const FetchResult &result);

    QString generateDdl(const QString &fullTableName, OraExp::TableType tableType, bool storage=true, bool tablespaceOnly=false) const;
    QString generateDropDdl(const QString &schemaName) const;
    QList< NameQueryPair > generateDiffDdl(const IndexInfo &other,
                                                    const QString &schemaName,
                                                    const QString &tableName,
                                                    OraExp::TableType tableType,
                                                    bool includePartitioningDiff=true,
                                                    bool includeStorageDiff=true) const;
    bool needsRecreation(const IndexInfo &other) const;
};

Q_DECLARE_METATYPE(IndexInfo)
Q_DECLARE_METATYPE(QList<IndexInfo>)

#endif // INDEXINFO_H
