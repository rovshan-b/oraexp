#include "htmlexporter.h"
#include "util/strutil.h"
#include "util/filesystemutil.h"
#include <QTextCodec>

HtmlExporter::HtmlExporter()
{
    this->isXmlFile=true;
}

void HtmlExporter::startDocument(QTextStream &out)
{
    out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n<html>\n<head><meta http-equiv=\"Content-type\" content=\"text/html;charset=" << this->encoding << "\"></head>\n";

    QString style = FileSystemUtil::readAsString(":/misc/html_export.css");
    out << "<style>\n";
    out << style;
    out << "</style>\n";

    out << "<body>\n<table border='1'>";
}

void HtmlExporter::prepareField(QString &fieldValue, int fieldIx)
{
    DataExporterBase::prepareField(fieldValue, fieldIx);

    fieldValue.replace("\n", "<br>");
}

void HtmlExporter::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    out << lineEnding << "   <thead>";
    out << lineEnding << "   <tr>";

    QString s;
    for(int i=from; i<=to; ++i){
        s = headers.at(i);

        out << lineEnding << "    <th>";
        out << xmlEncode(removeEnclosure(s));
        out << "</th>";
    }

    out << lineEnding << "   </tr>";
    out << lineEnding << "   </thead>";
    out << lineEnding << "   <tbody>";
}

void HtmlExporter::exportRow(const QStringList &oneRow, int /*rowIx*/, QTextStream &out)
{
    out << lineEnding << "   <tr>";

    for(int i=0; i<oneRow.size(); ++i){
        out << lineEnding << "    <td>";
        out << oneRow.at(i);
        out << "</td>";
    }

    out << lineEnding << "   </tr>";
}

void HtmlExporter::endDocument(QTextStream &out)
{
    out << lineEnding << "  </tbody>";
    out << lineEnding << "  </table>" << lineEnding << " </body>\n</html>";
}
