#include "resultsetcolumnmetadata.h"
#include <QStringList>
#include <stdexcept>
#include <sstream>

ResultsetColumnMetadata::ResultsetColumnMetadata()
{
}

int ResultsetColumnMetadata::getColumnCount() const
{
    return columnTitles.size();
}

unsigned int ResultsetColumnMetadata::getColumnIndexByName(const QString &colName) const
{
    if(columnTitles.contains(colName)){
        return columnTitles.indexOf(colName)+1;
    }else{
        return -1;
    }
}

bool ResultsetColumnMetadata::isTextColumn(unsigned int colIx) const
{
    return textColIndexes.contains(colIx);
}

OraExp::ColumnDataType ResultsetColumnMetadata::getColumnDataType(unsigned int colIx) const
{
    return columnDataTypes.value(colIx);
}
