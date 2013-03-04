#include "dataexporterbase.h"

DataExporterBase::DataExporterBase()
{
}

DataExporterBase::~DataExporterBase()
{
    qDeleteAll(stringReplacements);
}

void DataExporterBase::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    Q_UNUSED(headers);
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(out);
}
