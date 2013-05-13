#ifndef COLUMNMAPPING_H
#define COLUMNMAPPING_H

#include <QString>

class ColumnMapping
{
public:
    ColumnMapping();

    QString columnName;
    int fileFieldIx;
    QString fileFieldName;
    QString dateFormat;

};

#endif // COLUMNMAPPING_H
