#ifndef TABLEINFO_H
#define TABLEINFO_H

#include "dbobjectinfo.h"
#include "tablegeneralinfo.h"
#include "columninfo.h"
#include "constraintinfo.h"
#include "indexinfo.h"
#include "tableexternalinfo.h"
#include "triggerinfo.h"
#include "grantinfo.h"
#include "beans/querylistitem.h"
#include "tablecreateddloptions.h"
#include <QSharedPointer>

class TableInfo : public DbObjectInfo
{
public:
    TableInfo();
    ~TableInfo();

    TableGeneralInfo generalInfo;
    QList<ColumnInfo> columns;
    QList<ConstraintInfo> constraints;
    QList<ConstraintInfo> checkConstraints;
    QList<IndexInfo> indexes;
    TableExternalInfo externalInfo;
    TablePartitioningInfo partitioningInfo;
    QList<TriggerInfo> triggers;
    QList<TableGrantInfo> grants;

    void setSchemaName(const QString &newSchemaName);
    ColumnInfo findColumnByName(const QString &columnName, bool &found) const;
    ConstraintInfo findConstraintByColumns(OraExp::ConstraintType constraintType, const QString &constraintColumns, bool &found) const;
    IndexInfo findIndexByColumns(const QString &indexColumns, bool &found) const;
    TableGrantInfo findGrantByGrantee(const QString &grantee, bool &found) const;
    TriggerInfo findTriggerByName(const QString &triggerName, bool &found) const;

    void setLobTablespace(const QString &lobTablespace);
    void setPartitionTablespace(const QString &partitionTablespace);
    void prepareForOptions(const NewDbObjectDdlOptions &options);

    TableInfo *originalInfo() const;
    void setOriginalInfo(TableInfo *originalTableInfo);

    QString generateDdl(bool includeInlinePkConstraint=true,
                        bool includeConstraints=true,
                        QList<OraExp::ConstraintType> constraintTypes=QList<OraExp::ConstraintType>(),
                        bool includeGrants=true,
                        const TableCreateDdlOptions &options=TableCreateDdlOptions()) const;

    QList<QueryListItem> generateAlterDdl(const QHash<QString,QObject*> &requesters) const;

    void removeGeneratedNotNullConstraints();

    bool needsRecreation(const TableInfo &other, bool checkPartitioning) const;
private:
    TableInfo *originalTableInfo;
};

Q_DECLARE_METATYPE(TableInfo)

#endif // TABLEINFO_H
