#ifndef DATAEXPORTERFACTORY_H
#define DATAEXPORTERFACTORY_H

#include "exporters/dataexporterbase.h"

class DataExporterFactory
{
public:
    static DataExporterBase *createExporter(DataExporterBase::ExportFormat format);
    static DataExporterBase *createExporter(const QString &formatName);

private:
    DataExporterFactory();
};

#endif // DATAEXPORTERFACTORY_H
