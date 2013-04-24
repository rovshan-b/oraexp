#ifndef SCHEMAEXPORTOPTIONS_H
#define SCHEMAEXPORTOPTIONS_H

#include "dbobjectcomparisonoptions.h"
#include "tablecreateddloptions.h"
#include "sourcecodeddloptions.h"
#include "sequencecreateddloptions.h"

class SchemaExportOptions : public DbObjectComparisonOptions
{
public:
    SchemaExportOptions();

    QString filename;
    QString encoding;
    bool bom;

    TableCreateDdlOptions tableOptions;
    SourceCodeDdlOptions sourceCodeOptions;
    SequenceCreateDdlOptions sequenceOptions;
};

#endif // SCHEMAEXPORTOPTIONS_H
