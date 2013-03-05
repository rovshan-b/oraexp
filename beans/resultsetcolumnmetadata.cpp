#include "resultsetcolumnmetadata.h"
#include <stdexcept>
#include <sstream>

ResultsetColumnMetadata::ResultsetColumnMetadata()
{
}

int ResultsetColumnMetadata::getColumnCount() const
{
    return columnIndexes.size();
}

unsigned int ResultsetColumnMetadata::getColumnIndexByName(const QString &colName) const
{
    if(columnIndexes.contains(colName)){
        return columnIndexes.value(colName);
    }else{
        throw std::logic_error(QObject::tr("Column does not exist in resultset - %1").arg(colName).toStdString());
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
