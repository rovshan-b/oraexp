#include "dataexporterbase.h"
#include "beans/resultsetcolumnmetadata.h"

DataExporterBase::DataExporterBase()
{
}

DataExporterBase::~DataExporterBase()
{
    qDeleteAll(stringReplacements);
}

void DataExporterBase::prepareColumnHeaders(QStringList &headers)
{
    if(!includeColumnHeaders || !quoteColumnHeaders){
        return;
    }

    QString quotingSymbol = stringQuoting.isEmpty() ? "\"" : stringQuoting;

    for(int i=0; i<headers.size(); ++i){
        QString &s = headers[i];
        if(!s.startsWith(quotingSymbol)){
            s.prepend(quotingSymbol);
        }
        if(!s.endsWith(quotingSymbol)){
            s.append(quotingSymbol);
        }
    }
}

void DataExporterBase::prepareRow(QStringList &row, ResultsetColumnMetadata *columnMetadata)
{
    for(int i=0; i<row.size(); ++i){
        QString &s=row[i];

        if(!stringReplacements.isEmpty()){
            for(int i=0; i<stringReplacements.size(); ++i){
                QPair<QString,QString>* pair=stringReplacements.at(i);
                s.replace(pair->first, pair->second);
            }
        }

        if(includeNullText && s.isEmpty()){
            s="NULL";
        }

        if(!stringQuoting.isEmpty() &&
                columnMetadata->isTextColumn(i+1)){
            s.prepend(stringQuoting);
            s.append(stringQuoting);
        }else if(!numberQuoting.isEmpty() &&
                 columnMetadata->getColumnDataType(i+1)==OraExp::CDTNumeric){
            s.prepend(numberQuoting);
            s.append(numberQuoting);
        }
    }
}

void DataExporterBase::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    Q_UNUSED(headers);
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(out);
}
