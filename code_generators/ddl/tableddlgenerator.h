#ifndef TABLEDDLGENERATOR_H
#define TABLEDDLGENERATOR_H

#include <QList>
#include "beans/querylistitem.h"
#include "beans/tableinfo.h"
#include "beans/tablecreateddloptions.h"
#include "beans/tablediffddloptions.h"

class TableDdlGenerator
{
public:
    TableDdlGenerator();

    static QString generateDdl(const TableInfo &tableInfo,
                               bool includeInlinePkConstraint=true,
                               bool includeConstraints=true,
                               QList<OraExp::ConstraintType> constraintTypes=QList<OraExp::ConstraintType>(),
                               bool includeGrants=true,
                               const TableCreateDdlOptions &option=TableCreateDdlOptions());
    static QString generateConstraintsDdl(const TableInfo &tableInfo,
                                          QList<OraExp::ConstraintType> constraintTypes=QList<OraExp::ConstraintType>(),
                                          bool generateAsAlterTable=false);

    static QList<QueryListItem> generateAlterDdl(const TableInfo &tableInfo, const QHash<QString,QObject*> &requesters);

    static QPair<QString, QString> generateDdlForExporting(const TableInfo &tableInfo, const TableCreateDdlOptions &options);
    static QPair<QString, QString> generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const TableDiffDdlOptions &options);
    static QString generateDropDdl(const TableInfo &tableInfo);
};

#endif // TABLEDDLGENERATOR_H
