#ifndef INDEXORGANIZEDTABLEPROPERTIES_H
#define INDEXORGANIZEDTABLEPROPERTIES_H

#include "storageparams.h"
#include <QString>

class IndexOrganizedTableProperties
{
public:
    IndexOrganizedTableProperties();

    int pctThreshold;
    int mappingTable;
    int keyCompression;
    int keyCompressionPrefixLength;
    bool overflow;
    QString including;
    StorageParams storageParams;
    int logging;

    QString generateDdl() const;
    QString generateDiffDdl(const IndexOrganizedTableProperties &other) const;

    static IndexOrganizedTableProperties fromFetchResult(const FetchResult &result);
};

#endif // INDEXORGANIZEDTABLEPROPERTIES_H
