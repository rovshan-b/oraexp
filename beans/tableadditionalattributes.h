#ifndef TABLEADDITIONALATTRIBUTES_H
#define TABLEADDITIONALATTRIBUTES_H

#include <QString>
#include "connectivity/fetchresult.h"
#include "enums.h"

class TableAdditionalAttributes
{
public:
    TableAdditionalAttributes();

    int parallel;
    int parallelDegree;
    int logging;
    int cache;
    int monitoring;
    int compress;
    int rowDependencies;
    int rowMovement;
    int flashbackArchive;
    QString flashbackArchiveName;

    QString generateDdl(OraExp::TableType tableType, bool isPartitioned, bool fbArchive=true) const;
    QString generateDiffDdl(const TableAdditionalAttributes &other, bool fbAchive=true) const;

    static TableAdditionalAttributes fromFetchResult(const FetchResult &result);

};

#endif // TABLEADDITIONALATTRIBUTES_H
