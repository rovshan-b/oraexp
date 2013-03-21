#include "datacomparisonoptions.h"

DataComparisonOptions::DataComparisonOptions() :
    inserts(true),
    updates(true),
    deletes(true),
    includeSchemaName(true),
    disableRefConstraints(Auto),
    comparisonMode(GenerateDml)
{
}
