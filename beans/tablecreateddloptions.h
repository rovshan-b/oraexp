#ifndef TABLECREATEDDLOPTIONS_H
#define TABLECREATEDDLOPTIONS_H

#include <QString>
#include "newdbobjectddloptions.h"

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

    NewDbObjectDdlOptions newObjectOptions;
};

#endif // TABLECREATEDDLOPTIONS_H
