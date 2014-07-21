#include "scopeinfo.h"

ScopeInfo::ScopeInfo() :
    maxParamCount(-1)
{
}

ScopeInfo::ScopeInfo(const QString &scopeName, int maxParamCount) :
    scopeName(scopeName),
    maxParamCount(maxParamCount)
{
}
