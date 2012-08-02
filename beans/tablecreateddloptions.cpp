#include "tablecreateddloptions.h"

TableCreateDdlOptions::TableCreateDdlOptions() :
    properties(true),
    flashbackArchive(true),
    iotProperties(true),
    lobProperties(true),
    indexes(true),
    triggers(true),
    grants(true)
{
}
