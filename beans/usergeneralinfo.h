#ifndef USERGENERALINFO_H
#define USERGENERALINFO_H

#include <QString>

class UserGeneralInfo
{
public:
    enum UserIdentifiedBy
    {
        Password,
        Externally,
        Globally
    };

    UserGeneralInfo();

    QString username;
    UserIdentifiedBy identifiedBy;
    QString password;
    QString dn;
    QString defaultTablespace;
    QString temporaryTablespace;
    QString profile;
    bool expirePassword;
    bool accountLocked;
    bool enableEditions;
};

#endif // USERGENERALINFO_H
