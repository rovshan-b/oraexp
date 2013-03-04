#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include "dataexporterbase.h"

class CsvExporter : public DataExporterBase
{
public:
    CsvExporter();

    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, QTextStream &out);
};

#endif // CSVEXPORTER_H
