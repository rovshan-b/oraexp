#ifndef ROLEGRANTINFO_H
#define ROLEGRANTINFO_H

#include <QString>
#include "defines.h"
#include "connectivity/fetchresult.h"

class PrivGrantInfo
{
public:
    PrivGrantInfo();

    bool operator==(const PrivGrantInfo &other) const;

    int grantId;
    QString name;
    bool isGrantable;
    bool isDefault;

    bool markedForDeletion;
    bool dropped;

    QString generateDdl(const QString &username) const;
    QString generateDropDdl(const QString &username) const;
    QList< NameQueryPair > generateDiffDdl(const PrivGrantInfo &other,
                                           const QString &username) const;
    bool needsRecreation(const PrivGrantInfo &other) const;

    static PrivGrantInfo fromFetchResult(const FetchResult &result);
};

#endif // ROLEGRANTINFO_H
