#include "csvexporter.h"

CsvExporter::CsvExporter()
{
}

void CsvExporter::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    for(int i=from; i<=to; ++i){
        out << headers.at(i);

        if(i==to){
            out << this->lineEnding;
        }else{
            out << this->columnDelimiter;
        }
    }
}

void CsvExporter::exportRow(const QStringList &oneRow, QTextStream &out)
{
    for(int i=0; i<oneRow.size(); ++i){
        out << oneRow.at(i);

        if(i==oneRow.size()-1){
            out << this->lineEnding;
        }else{
            out << this->columnDelimiter;
        }
    }
}
