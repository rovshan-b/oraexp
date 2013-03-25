#ifndef DATACOMPARISONOPTIONS_H
#define DATACOMPARISONOPTIONS_H

#include "dataoperationoptions.h"

class DataComparisonOptions : public DataOperationOptions
{
public:
    DataComparisonOptions();

    bool inserts;
    bool updates;
    bool deletes;
    bool includeSchemaName;
};

#endif // DATACOMPARISONOPTIONS_H
