#ifndef SYNONYMINFO_H
#define SYNONYMINFO_H

#include "defines.h"
#include "dbobjectinfo.h"
#include <QString>
#include "connectivity/fetchresult.h"

class SynonymInfo : public DbObjectInfo
{
public:
    SynonymInfo();

    QString owner;
    QString name;

    QString targetSchema;
    QString targetObject;
    bool overDbLink;
    QString dbLinkName;

    bool dropped;

    QString generateDdl(bool replace) const;
    QList<NameQueryPair> generateDiffDdl(const SynonymInfo &other) const;
    QString generateDropDdl() const;
    QStringList validate() const;
    static SynonymInfo fromFetchResult(const FetchResult &result);

    bool needsRecreation(const SynonymInfo &other) const;
};

#endif // SYNONYMINFO_H
