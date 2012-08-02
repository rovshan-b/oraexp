#ifndef FETCHRESULT_H
#define FETCHRESULT_H

#include <QList>
#include <QHash>
#include "ociexception.h"

class Resultset;

class FetchResult
{
public:
    FetchResult();

    QString taskName;

    QHash<QString, unsigned int> columnIndexes;
    QList<QString> oneRow;

    bool hasError;
    OciException exception;

    QString colValue(const QString &columnName) const {
        int colIx=columnIndexes.value(columnName)-1;
        return colIx<0 ? "" : oneRow.at(colIx);
    }
    int colValue(const QString &columnName, int defaultValue) const {
        QString val = colValue(columnName);
        return val.isEmpty() ? defaultValue : val.toInt();
    }
};

#endif // FETCHRESULT_H
