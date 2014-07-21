#ifndef SCOPEINFO_H
#define SCOPEINFO_H

#include <QString>

class ScopeInfo
{
public:
    ScopeInfo();
    ScopeInfo(const QString &scopeName, int maxParamCount);

    QString scopeName;
    int maxParamCount;
};

#endif // SCOPEINFO_H
