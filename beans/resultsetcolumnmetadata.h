#ifndef RESULTSETCOLUMNMETADATA_H
#define RESULTSETCOLUMNMETADATA_H

#include <QHash>
#include "enums.h"

class ResultsetColumnMetadata
{
public:
    ResultsetColumnMetadata();

    QHash<QString, unsigned int> columnIndexes;
    QHash<unsigned int, OraExp::ColumnDataType> columnDataTypes;
    QList<unsigned int> textColIndexes;

    int getColumnCount() const;
    unsigned int getColumnIndexByName(const QString &colName) const;
    bool isTextColumn(unsigned int colIx) const;
    OraExp::ColumnDataType getColumnDataType(unsigned int colIx) const;
};

#endif // RESULTSETCOLUMNMETADATA_H
