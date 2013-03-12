#include "dataexporterbase.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/strutil.h"
#include <QFile>

DataExporterBase::DataExporterBase() : isXmlFile(false), textStream(0), file(0)
{
}

DataExporterBase::~DataExporterBase()
{
    qDeleteAll(stringReplacements);

    delete textStream;
    delete file;
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

void DataExporterBase::prepareRow(QStringList &row)
{
    for(int i=0; i<row.size(); ++i){
        QString &s=row[i];

        prepareField(s, i);
    }
}

void DataExporterBase::prepareField(QString &fieldValue, int fieldIx)
{
    if(!stringReplacements.isEmpty()){
        for(int i=0; i<stringReplacements.size(); ++i){
            QPair<QString,QString>* pair=stringReplacements.at(i);
            fieldValue.replace(pair->first, pair->second);
        }
    }

    if(includeNullText && fieldValue.isEmpty()){
        fieldValue="NULL";
    }

    if(!stringQuoting.isEmpty() &&
            columnMetadata->isTextColumn(startColumn+fieldIx+1)){
        fieldValue.replace(stringQuoting, QString("\\%1").arg(stringQuoting)); //first escape quoting characters inside string
        fieldValue.prepend(stringQuoting);
        fieldValue.append(stringQuoting);
    }else if(!numberQuoting.isEmpty() &&
             columnMetadata->getColumnDataType(startColumn+fieldIx+1)==OraExp::CDTNumeric){
        fieldValue.prepend(numberQuoting);
        fieldValue.append(numberQuoting);
    }

    if(this->isXmlFile){
        fieldValue = xmlEncode(fieldValue);
    }
}

void DataExporterBase::exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out)
{
    Q_UNUSED(headers);
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(out);
}

QTextStream *DataExporterBase::createOutputStream(QString &errorMessage)
{
    Q_ASSERT(file==0);

    file = new QFile(this->filename);
    if(!file->open(QIODevice::WriteOnly)){
        errorMessage = file->errorString();
        return 0;
    };

    textStream = new QTextStream(file);
    setTextStreamProperties();

    return textStream;
}

void DataExporterBase::setTextStreamProperties()
{
    textStream->setCodec(this->encoding.toStdString().c_str());
    if(this->encoding.startsWith("UTF")){
        textStream->setGenerateByteOrderMark(this->bom);
    }
}
