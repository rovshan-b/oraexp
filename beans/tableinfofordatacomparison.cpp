#include "tableinfofordatacomparison.h"

TableInfoForDataComparison::TableInfoForDataComparison()
{
}

void TableInfoForDataComparison::addColumnToCompare(const QString &name)
{
    //columns.append(name);

    columnsToCompare.append(name);
}
