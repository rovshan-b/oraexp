#ifndef RESULTSETCOLUMNMETADATA_H
#define RESULTSETCOLUMNMETADATA_H

#include <QHash>
#include <QStringList>
#include "enums.h"

class ResultsetColumnMetadata
{
public:
    ResultsetColumnMetadata();

    QStringList columnTitles;
    QHash<unsigned int, OraExp::ColumnDataType> columnDataTypes;
    QHash<unsigned int, OraExp::ColumnSubType> columnSubTypes;
    QList<unsigned int> textColIndexes;

    int getColumnCount() const;
    unsigned int getColumnIndexByName(const QString &colName) const;
    bool isTextColumn(unsigned int colIx) const;
    OraExp::ColumnDataType getColumnDataType(unsigned int colIx) const;
};

#endif // RESULTSETCOLUMNMETADATA_H
