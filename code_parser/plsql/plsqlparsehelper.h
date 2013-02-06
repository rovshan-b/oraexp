#ifndef PLSQLPARSEHELPER_H
#define PLSQLPARSEHELPER_H

#include <QStringList>
#include "beans/bindparaminfo.h"

class PlSqlParseHelper
{
public:
    static QStringList getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes=0);

    static bool isDml(const QString &query);

private:
    PlSqlParseHelper();
};

#endif // PLSQLPARSEHELPER_H
