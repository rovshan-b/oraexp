#include "csvexporter.h"

CsvExporter::CsvExporter()
{
}

void CsvExporter::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    exportStringList(headers, from, to, out);
}

void CsvExporter::exportRow(const QStringList &oneRow, QTextStream &out)
{
    exportStringList(oneRow, 0, oneRow.size()-1, out);
}

void CsvExporter::exportStringList(const QStringList &list, int from, int to, QTextStream &out)
{
    for(int i=from; i<=to; ++i){
        out << list.at(i);

        if(i==to){
            if(this->delimiterAfterLastColumn){
                out << this->columnDelimiter;
            }
            out << this->lineEnding;
        }else{
            out << this->columnDelimiter;
        }
    }
}
