#include "dataexporteroptions.h"
#include "connection_page/data_exporter/exporters/dataexporterbase.h"

DataExporterOptions::DataExporterOptions() : exporter(0)
{
}

DataExporterOptions::~DataExporterOptions()
{
    delete exporter;
}
