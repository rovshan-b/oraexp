#ifndef SCHEMACOMPARISONOPTIONS_H
#define SCHEMACOMPARISONOPTIONS_H

#include "dbobjectcomparisonoptions.h"
#include "tablecreateddloptions.h"
#include "tablediffddloptions.h"
#include "newdbobjectstorageoptions.h"
#include "sourcecodeddloptions.h"
#include "sequencecreateddloptions.h"
#include "sequencediffddloptions.h"

class SchemaComparisonOptions : public DbObjectComparisonOptions
{
public:
    SchemaComparisonOptions();

    TableCreateDdlOptions tableCreateOptions;
    TableDiffDdlOptions tableDiffOptions;
    SourceCodeDdlOptions sourceCodeOptions;
    SequenceCreateDdlOptions sequenceCreateOptions;
    SequenceDiffDdlOptions sequenceDiffOptions;
};

#endif // SCHEMACOMPARISONOPTIONS_H
