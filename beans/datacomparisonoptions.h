#ifndef DATACOMPARISONOPTIONS_H
#define DATACOMPARISONOPTIONS_H

#include "beans/dbobjectcomparisonoptions.h"

class DataComparisonOptions : public DbObjectComparisonOptions
{
public:
    enum ComparisonMode
    {
        GenerateDml=0,
        UpdateDatabase=1
    };

    enum DisableRefConstraintsMode
    {
        Auto,
        Disable,
        DoNotDisable
    };

    DataComparisonOptions();

    bool inserts;
    bool updates;
    bool deletes;
    bool includeSchemaName;
    DisableRefConstraintsMode disableRefConstraints;
    ComparisonMode comparisonMode;
};

#endif // DATACOMPARISONOPTIONS_H
