#ifndef BINDPARAMINFO_H
#define BINDPARAMINFO_H

#include <QStringList>
#include "util/param.h"

class BindParamInfo
{
public:
    enum BindParamType
    {
        StringOrNumber,
        Date,
        Cursor
    };

    BindParamInfo();
    BindParamInfo(BindParamType paramType, Param::ParamDirection paramDirection);

    BindParamType paramType;
    QStringList paramValueHistory;
    Param::ParamDirection paramDirection;
};

#endif // BINDPARAMINFO_H
