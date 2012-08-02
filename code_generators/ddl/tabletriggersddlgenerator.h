#ifndef TABLETRIGGERSDDLGENERATOR_H
#define TABLETRIGGERSDDLGENERATOR_H

#include "beans/tableinfo.h"

class TableTriggersDdlGenerator
{
public:
    TableTriggersDdlGenerator();

    static QString generateDdl(const TableInfo &tableInfo);
    static QString generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo);
};

#endif // TABLETRIGGERSDDLGENERATOR_H
