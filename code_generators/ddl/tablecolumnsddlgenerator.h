#ifndef TABLECOLUMNSDDLGENERATOR_H
#define TABLECOLUMNSDDLGENERATOR_H

#include <QList>
#include "beans/columninfo.h"
#include "beans/tableinfo.h"

class TableColumnsDdlGenerator
{
public:
    TableColumnsDdlGenerator();

    static QString generateDdl(const TableInfo &tableInfo, bool includeInlinePkConstraint=true);
    static QList< NameQueryPair > generateAlterDdl(const TableInfo &tableInfo);

    static QString generateAppendDdl(const TableInfo &tableInfo,
                                     int rowIx,
                                     bool includeInlinePkUqConstraints,
                                     bool lobParams=true, bool storage=true, bool tablespaceOnly=false);
    static QString generateLobParamsDdl(const TableInfo &tableInfo);
    static QString generateLobParamsDdlForRow(const TableInfo &tableInfo, int row, bool storage=true, bool tablespaceOnly=false);
    static QString generateCommentsDdl(const TableInfo &tableInfo);
    static QString generateCommentsDdlForRow(const TableInfo &tableInfo, const QString &fullTableName, int row);
    static QString generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, QStringList &droppedColumnNames, bool comments, bool lobParams, bool storageForNew, bool tablespaceOnlyForNew);
};

#endif // TABLECOLUMNSDDLGENERATOR_H
