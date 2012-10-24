#ifndef DBLINKINFO_H
#define DBLINKINFO_H

#include "dbobjectinfo.h"
#include "connectivity/fetchresult.h"
#include "defines.h"

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
    bool dropped;

    QString generateDdl() const;
    QString generateDropDdl() const;
    QList<NameQueryPair> generateDiffDdl(const DbLinkInfo &other, bool canAlter) const;
    bool needsRecreation(const DbLinkInfo &other, bool canAlter) const;
    QStringList validate(bool editMode) const;
    static DbLinkInfo fromFetchResult(const FetchResult &result);
};

#endif // DBLINKINFO_H
