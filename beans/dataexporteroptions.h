#ifndef DATAEXPORTEROPTIONS_H
#define DATAEXPORTEROPTIONS_H

#include "beans/dataoperationoptions.h"

class DataExporterBase;

class DataExporterOptions : public DataOperationOptions
{
public:
    DataExporterOptions();
    virtual ~DataExporterOptions();

    DataExporterBase *exporter;

    bool singleFile;
};

#endif // DATAEXPORTEROPTIONS_H
