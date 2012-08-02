#ifndef SEQUENCEINFO_H
#define SEQUENCEINFO_H

#include "dbobjectinfo.h"
#include "connectivity/fetchresult.h"
#include "beans/sequencecreateddloptions.h"
#include "beans/sequencediffddloptions.h"
#include <QString>

class SequenceInfo : public DbObjectInfo
{
public:
    SequenceInfo();
    virtual ~SequenceInfo();

    QString owner;
    QString name;
    QString minValue;
    QString maxValue;
    QString incrementBy;
    bool cycle;
    bool ordered;
    QString cacheSize;
    QString lastNumber;

    QString generateDdl(const SequenceCreateDdlOptions &options) const;
    QString generateDropDdl() const;
    QString generateDiffDdl(const SequenceInfo &other, const SequenceDiffDdlOptions &options) const;

    static SequenceInfo fromFetchResult(const FetchResult &result);
};

#endif // SEQUENCEINFO_H
