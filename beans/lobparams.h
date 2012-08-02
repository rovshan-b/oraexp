#ifndef LOBPARAMS_H
#define LOBPARAMS_H

#include "storageparams.h"

class LobParams
{
public:
    LobParams();

    bool operator==(const LobParams &other) const;

    QString generateDdl(bool storage=true, bool tablespaceOnly=false) const;
    QString generateDiffDdl(const LobParams &other) const;

    StorageParams storageParams;

    QString segment;
    QString tablespaceName;
    int cache;
    int logging;
    int chunk;
    int pctVersion;
    int freePools;
    bool enableStorageInRow;
    bool enableRetention;

    bool isEmpty;

    static LobParams fromFetchResult(const FetchResult &result);
};

Q_DECLARE_METATYPE(LobParams)

#endif // LOBPARAMS_H
