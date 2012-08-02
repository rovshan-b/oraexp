#include "tablediffddloptions.h"

TableDiffDdlOptions::TableDiffDdlOptions() :
    comments(true),
    properties(true),
    flashbackArchive(true),
    storage(true),
    iotProperties(true),
    lobProperties(true),
    columns(true),
    constraints(true),
    indexes(true),
    partitions(true),
    externalProperties(true),
    triggers(true),
    grants(true)
{
}
