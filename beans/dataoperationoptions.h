#ifndef DATAOPERATIONOPTIONS_H
#define DATAOPERATIONOPTIONS_H

#include "beans/dbobjectcomparisonoptions.h"

class DataOperationOptions : public DbObjectComparisonOptions
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
        DisableForAll,
        DoNotDisable
    };

    DataOperationOptions();

    DisableRefConstraintsMode disableRefConstraints;
    ComparisonMode comparisonMode;
    bool uncheckInGuiOnSuccess;
    bool skipOnError;
};

#endif // DATAOPERATIONOPTIONS_H
