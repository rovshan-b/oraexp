#ifndef STORAGEPARAMS_H
#define STORAGEPARAMS_H

#include <QString>
#include <QMetaType>
#include "connectivity/fetchresult.h"
#include "enums.h"

class StorageParams
{
public:
    StorageParams();

    bool operator==(const StorageParams &other) const;

    QString tablespaceName;
    int pctFree;
    int pctUsed;
    int initTrans;
    int maxTrans;

    int initialExtent;
    OraExp::ExtentUnit initialExtentUnit;
    int nextExtent;
    OraExp::ExtentUnit nextExtentUnit;
    int pctIncrease;
    int minExtents;
    int maxExtents;
    bool unlimitedMaxExtents;
    int maxSize;
    OraExp::ExtentUnit maxSizeUnit;
    bool unlimitedMaxSize;
    int freeLists;
    int freeListGroups;
    QString bufferPool;

    bool isEmpty;

    QString generateDdl(bool configureForLob=false, bool tablespaceOnly=false) const;
    QString generateDiffDdl(const StorageParams &other, bool configureForIndex=false) const;

    static StorageParams fromFetchResult(const FetchResult &result);

private:
    QString getSizeUnitText(OraExp::ExtentUnit unit) const;
};

Q_DECLARE_METATYPE(StorageParams)

#endif // STORAGEPARAMS_H
