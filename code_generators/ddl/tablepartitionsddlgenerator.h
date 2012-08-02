#ifndef TABLEPARTITIONSDDLGENERATOR_H
#define TABLEPARTITIONSDDLGENERATOR_H

#include "beans/tableinfo.h"

class TablePartitionsDdlGenerator
{
public:
    TablePartitionsDdlGenerator();

    static QString generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, bool configureForIndex);

private:
    static QString generatePartitionsDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, bool configureForIndex);
    static void generateSubpartitionsDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const QString &partitionName, QString &ddl);
};

#endif // TABLEPARTITIONSDDLGENERATOR_H
