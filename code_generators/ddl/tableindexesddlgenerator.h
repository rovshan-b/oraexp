#ifndef TABLEINDEXESDDLGENERATOR_H
#define TABLEINDEXESDDLGENERATOR_H

#include "beans/tableinfo.h"

class TableIndexesDdlGenerator
{
public:
    TableIndexesDdlGenerator();

    static QString generateDdl(const TableInfo &tableInfo, bool storage=true, bool tablespaceOnly=false);
    static QList<NameQueryPair> generateAlterDdl(const TableInfo &tableInfo);
    static QString generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const QStringList &droppedColumnNames,
                                   bool storageForNew=true, bool tablespaceOnlyForNew=false);
};

#endif // TABLEINDEXESDDLGENERATOR_H
