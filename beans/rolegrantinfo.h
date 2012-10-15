#ifndef ROLEGRANTINFO_H
#define ROLEGRANTINFO_H

#include <QString>

class RoleGrantInfo
{
public:
    RoleGrantInfo();

    int roleId;
    QString roleName;
    bool isGrantable;
    bool isDefault;
    bool markedForDeletion;
};

#endif // ROLEGRANTINFO_H
