#ifndef PLSQLPARSEHELPER_H
#define PLSQLPARSEHELPER_H

#include <QStringList>
#include "beans/bindparaminfo.h"

class PlSqlParseHelper
{
public:
    static QStringList getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes=0);

    static QString getFirstLexeme(const QString &query);
    static bool isDml(const QString &query);

    static void getNextQueryPos(const QString &query, int startFromPos, int *queryStartPos, int *queryEndPos);

private:
    PlSqlParseHelper();
};

#endif // PLSQLPARSEHELPER_H
