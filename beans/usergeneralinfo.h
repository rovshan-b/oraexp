#ifndef USERGENERALINFO_H
#define USERGENERALINFO_H

#include <QString>
#include "defines.h"
#include "connectivity/fetchresult.h"

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

    QString generateDdl() const;
    QString generateDropDdl() const;
    QList< NameQueryPair > generateDiffDdl(const UserGeneralInfo &other) const;
    static UserGeneralInfo fromFetchResult(const FetchResult &result);

private:
    QString getIdentifiedBy() const;
};

#endif // USERGENERALINFO_H
