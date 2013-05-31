#ifndef TRIGGERINFO_H
#define TRIGGERINFO_H

#include "connectivity/fetchresult.h"
#include "defines.h"

class TriggerInfo
{
public:
    TriggerInfo();

    QString owner;
    QString name;
    QString type;
    QString triggeringEvent;
    QString baseObjectType;
    bool forEachRow;
    QString columnName;
    QString referencingNames;
    QString crossEdition;
    QString precedes;
    QString follows;
    QString whenClause;
    bool enabled;
    //QString description;
    QString actionType;
    QString body;
    bool isEmpty;

    QString generateDdl(const QString &fullTableName, bool sqlTerminator) const;
    QString generateEnableDdl(const QString &fullTableName) const;
    QString generateDropDdl() const;
    QString generateDiffDdl(const TriggerInfo &other, const QString &fullTableName) const;

    static TriggerInfo fromFetchResult(const FetchResult &result);
};

#endif // TRIGGERINFO_H
