#include "bindparaminfo.h"

BindParamInfo::BindParamInfo()
{
}

BindParamInfo::BindParamInfo(BindParamType paramType, Param::ParamDirection paramDirection) :
    paramType(paramType), paramDirection(paramDirection)
{
}
