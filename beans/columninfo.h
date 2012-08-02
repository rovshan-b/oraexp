#ifndef COLUMNINFO_H
#define COLUMNINFO_H

#include <QString>
#include "enums.h"
#include "lobparams.h"
#include "defines.h"
#include <QMetaType>

class ColumnInfo
{
public:
    ColumnInfo();

    bool operator==(const ColumnInfo &other) const;

    int columnId;
    QString name;
    OraExp::ColumnType columnType;
    QString dataType;
    bool isNullable;
    QString defaultValue;
    QString virtualDefinition;
    bool isPrimaryKey;
    bool isUnique;
    QString comments;

    LobParams lobParams;

    bool markedForDeletion;
    bool dropped;

    QString generateDdl(OraExp::TableType tableType, bool includeInlinePkUqConstraints=true) const;
    QString generateDropDdl(const QString &fullTableName) const;
    QString generateRenameDdl(const QString &fullTableName, const QString &newName) const;
    QList< NameQueryPair > generateDiffDdl(const ColumnInfo &other,
                                                    const QString &fullTableName,
                                                    OraExp::TableType=OraExp::TableTypeHeap,
                                                    bool compareComments=true,
                                                    bool compareLobParams=true) const;
    bool needsRecreation(const ColumnInfo &) const {return false;}

    static ColumnInfo fromFetchResult(const FetchResult &result);
};

Q_DECLARE_METATYPE(ColumnInfo)

Q_DECLARE_METATYPE(QList<ColumnInfo>)

#endif // COLUMNINFO_H
