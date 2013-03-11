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
    out << "\n   <tr>";

    QString s;
    for(int i=from; i<=to; ++i){
        s = headers.at(i);

        out << "\n    <td><b>";
        out << xmlEncode(removeEnclosure(s));
        out << "</b></td>";
    }

    out << "\n   </tr>";
}

void HtmlExporter::exportRow(const QStringList &oneRow, int /*rowIx*/, QTextStream &out)
{
    out << "\n   <tr>";

    for(int i=0; i<oneRow.size(); ++i){
        out << "\n    <td>";
        out << oneRow.at(i);
        out << "</td>";
    }

    out << "\n   </tr>";
}

void HtmlExporter::endDocument(QTextStream &out)
{
    out << "\n  </table>\n </body>\n</html>";
}
