#ifndef SYSPRIVGRANTINFO_H
#define SYSPRIVGRANTINFO_H

#include <QString>

class SysPrivGrantInfo
{
public:
    SysPrivGrantInfo();

    int privId;
    QString privName;
    bool isGrantable;

    bool markedForDeletion;
};

#endif // SYSPRIVGRANTINFO_H
