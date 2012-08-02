#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QString>
#include <QMap>
#include <QVariant>
#include "enums.h"
#include "ociexception.h"

class Statement;

class QueryResult
{
public:
    QueryResult();

    Statement *statement; //must be released by owner

    QString taskName;
    QObject *requester;

    QString dbmsOutput;

    //these variables are only used for threaded use
    bool hasError;
    OciException exception;
};

#endif // QUERYRESULT_H
