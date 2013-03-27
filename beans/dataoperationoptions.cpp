#include "dataoperationoptions.h"

DataOperationOptions::DataOperationOptions() :
    disableRefConstraints(Auto),
    comparisonMode(GenerateDml),
    uncheckInGuiOnSuccess(true),
    skipOnError(false)
{
}
