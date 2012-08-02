#ifndef CONSTRAINTINFO_H
#define CONSTRAINTINFO_H

#include <QString>
#include "enums.h"
#include "connectivity/fetchresult.h"
#include "defines.h"
#include <QMetaType>

class ConstraintInfo
{
public:
    ConstraintInfo();

    bool operator==(const ConstraintInfo &other) const;

    int constraintId;
    QString name;
    OraExp::ConstraintType type;
    QString columns;
    QString searchCondition;
    QString rOwner;
    QString rTableName;
    QString rConsName;
    QString rColumns;
    int deleteRule;
    bool deferrable;
    bool deferred;
    bool generated;

    bool markedForDeletion;
    bool dropped;

    QString generateDdl(OraExp::TableType tableType, bool generateAsAlterTable=false, const QString &fullTableName=QString()) const;
    QString generateDropDdl(const QString &fullTableName) const;
    QList< NameQueryPair > generateDiffDdl(const ConstraintInfo &other,
                                                    const QString &fullTableName,
                                                    OraExp::TableType tableType) const;

    bool needsRecreation(const ConstraintInfo &other) const;

    static ConstraintInfo fromFetchResult(const FetchResult &result);
};

Q_DECLARE_METATYPE(ConstraintInfo)

Q_DECLARE_METATYPE(QList<ConstraintInfo>)

#endif // CONSTRAINTINFO_H
