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
