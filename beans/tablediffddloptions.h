#ifndef TABLEDIFFDDLOPTIONS_H
#define TABLEDIFFDDLOPTIONS_H

#include "newdbobjectddloptions.h"

class TableDiffDdlOptions
{
public:
    TableDiffDdlOptions();

    bool comments;
    bool properties;
    bool flashbackArchive;
    bool storage;
    bool iotProperties;
    bool lobProperties;
    bool columns;
    bool constraints;
    bool indexes;
    bool partitions;
    bool externalProperties;
    bool triggers;
    bool grants;

    NewDbObjectDdlOptions newObjectOptions;

};

#endif // TABLEDIFFDDLOPTIONS_H
