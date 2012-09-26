#ifndef DBLINKINFO_H
#define DBLINKINFO_H

#include "dbobjectinfo.h"
#include "connectivity/fetchresult.h"

#include <QString>

class DbLinkInfo : public DbObjectInfo
{
public:
    DbLinkInfo();
    virtual ~DbLinkInfo();

    QString owner;
    QString name;
    bool currentUser;
    QString username;
    QString password;
    QString host;

    bool shared;
    QString sharedAuthenticatedBy;
    QString sharedIdentifiedBy;

    bool isCompleteInfo;

    QString generateDdl() const;
    QStringList validate() const;
    static DbLinkInfo fromFetchResult(const FetchResult &result);
};

#endif // DBLINKINFO_H
