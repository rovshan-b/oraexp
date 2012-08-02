#ifndef TABLECONSTRAINTSDDLGENERATOR_H
#define TABLECONSTRAINTSDDLGENERATOR_H

#include <QList>
#include "beans/tableinfo.h"

class TableConstraintsDdlGenerator
{
public:
    TableConstraintsDdlGenerator();

    static QString generateKeyConstraintsDdl(const TableInfo &tableInfo,
                                             QList<OraExp::ConstraintType> constraintTypes=QList<OraExp::ConstraintType>(),
                                             bool generateAsAlterTable=false,
                                             const QString &fullTableName=QString());
    static QList<NameQueryPair> generateKeyConstraintsAlterDdl(const TableInfo &tableInfo);

    static QString generateCheckConstraintsDdl(const TableInfo &tableInfo, bool generateAsAlterTable=false, const QString &fullTableName=QString());
    static QList<NameQueryPair> generateCheckConstraintsAlterDdl(const TableInfo &tableInfo);
    static QString generateDiffDdl(QList<OraExp::ConstraintType> constraintTypes, const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const QStringList &droppedColumnNames);
};

#endif // TABLECONSTRAINTSDDLGENERATOR_H
