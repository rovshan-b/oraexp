#ifndef XMLEXPORTER_H
#define XMLEXPORTER_H

#include "dataexporterbase.h"

class XmlExporter : public DataExporterBase
{
public:
    XmlExporter();

    virtual void startDocument(QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out);
    virtual void endDocument(QTextStream &out);

    virtual QString getMimeType() const {return "text/xml";}
};

#endif // XMLEXPORTER_H
