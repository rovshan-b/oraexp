#ifndef PARTITIONINFO_H
#define PARTITIONINFO_H

#include <QString>
#include <QMetaType>
#include "connectivity/fetchresult.h"
#include "enums.h"
#include "defines.h"

class PartitioningInfo;

class PartitionInfo
{
public:
    PartitionInfo();

    bool operator==(const PartitionInfo &other) const;

    QString partitionName;
    QString subpartitionName;
    QString highValue;
    int partitionPosition;
    //int subpartitionPosition;
    QString tablespaceName;

    bool markedForDeletion;
    bool dropped;

    static PartitionInfo fromFetchResult(const FetchResult &result);

    QString generateDropDdl(const QString &fullTableName, OraExp::PartitionPart configureFor, bool configureForIndex) const;
    QString generateAppendDdl(const QString &fullTableName, OraExp::PartitionType pType, OraExp::PartitionPart configureFor, bool configureForIndex, const PartitioningInfo *subpartitionInfo) const;
    QString generateDdl(OraExp::PartitionType pType, OraExp::PartitionPart configureFor) const;
    QList< NameQueryPair > generateDiffDdl(const PartitionInfo &other,
                                                    const QString &fullTableName,
                                                    OraExp::PartitionPart configureFor,
                                                    bool configureForIndex) const;

    QString getNewListValues(const QString &originalValues, const QString &newValues) const;
    QString getDroppedListValues(const QString &originalValues, const QString &newValues) const;
    bool needsRecreation(const PartitionInfo &) const {return false;}
};


#endif // PARTITIONINFO_H
