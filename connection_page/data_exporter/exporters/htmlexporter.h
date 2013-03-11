#ifndef HTMLEXPORTER_H
#define HTMLEXPORTER_H

#include "dataexporterbase.h"

class HtmlExporter : public DataExporterBase
{
public:
    HtmlExporter();

    virtual void startDocument(QTextStream &out);
    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out);
    virtual void endDocument(QTextStream &out);
};

#endif // HTMLEXPORTER_H
