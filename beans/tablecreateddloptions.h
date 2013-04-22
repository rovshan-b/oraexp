#ifndef TABLECREATEDDLOPTIONS_H
#define TABLECREATEDDLOPTIONS_H

#include <QString>
#include "newdbobjectstorageoptions.h"

class TableCreateDdlOptions
{
public:
    TableCreateDdlOptions();

    bool properties;
    bool flashbackArchive;
    bool iotProperties;
    bool lobProperties;
    bool indexes;
    bool triggers;
    bool grants;

    NewDbObjectStorageOptions newObjectStorageOptions;
};

#endif // TABLECREATEDDLOPTIONS_H
