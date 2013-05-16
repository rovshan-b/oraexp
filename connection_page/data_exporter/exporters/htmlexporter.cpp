#include "htmlexporter.h"
#include "util/strutil.h"

HtmlExporter::HtmlExporter()
{
    this->isXmlFile=true;
}

void HtmlExporter::startDocument(QTextStream &out)
{
    out << "<html>\n <body>\n  <table border='1'>";
}

void HtmlExporter::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    out << lineEnding << "   <tr>";

    QString s;
    for(int i=from; i<=to; ++i){
        s = headers.at(i);

        out << lineEnding << "    <td><b>";
        out << xmlEncode(removeEnclosure(s));
        out << "</b></td>";
    }

    out << lineEnding << "   </tr>";
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
    out << lineEnding << "  </table>" << lineEnding << " </body>\n</html>";
}
