#include "xmlexporter.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/strutil.h"
#include <QTextCodec>

XmlExporter::XmlExporter()
{
    this->isXmlFile=true;
}

void XmlExporter::startDocument(QTextStream &out)
{
    out << QString("<?xml version=\"1.0\" encoding=\"%1\" standalone=\"yes\"?>").arg((QString)out.codec()->name());
    out << "\n<RECORDS>";
}

void XmlExporter::exportRow(const QStringList &oneRow, int /*rowIx*/, QTextStream &out)
{
    out << "\n <RECORD>";
    QString title;
    for(int i=0; i<oneRow.size(); ++i){
        title = xmlEncode(removeEnclosure(columnMetadata->columnTitles.at(i)));
        out << "\n  <" << title << ">";
        out << oneRow.at(i);
        out << "</" << title << ">";
    }
    out << "\n </RECORD>";
}

void XmlExporter::endDocument(QTextStream &out)
{
    out << "\n</RECORDS>";
}
