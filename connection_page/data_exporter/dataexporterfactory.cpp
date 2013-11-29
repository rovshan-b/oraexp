#include "dataexporterfactory.h"
#include "exporters/csvexporter.h"
#include "exporters/excelexporter.h"
#include "exporters/htmlexporter.h"
#include "exporters/xmlexporter.h"
#include "exporters/insertexporter.h"

DataExporterFactory::DataExporterFactory()
{
}

DataExporterBase *DataExporterFactory::createExporter(DataExporterBase::ExportFormat format)
{
    DataExporterBase *exporter;

    switch(format){
    case DataExporterBase::CSV:
        exporter=new CsvExporter();
        break;
    case DataExporterBase::Excel:
        exporter=new ExcelExporter();
        break;
    case DataExporterBase::HTML:
        exporter=new HtmlExporter();
        break;
    case DataExporterBase::XML:
        exporter=new XmlExporter();
        break;
    case DataExporterBase::Insert:
        exporter=new InsertExporter();
        break;
    default:
        exporter=0;
        Q_ASSERT(false);
        break;
    }

    return exporter;
}

DataExporterBase *DataExporterFactory::createExporter(const QString &formatName)
{
    DataExporterBase *exporter;

    if(formatName == QObject::tr("TSV")){
        exporter = new CsvExporter();
        exporter->columnDelimiter = "\t";
    }else if(formatName == QObject::tr("CSV")){
        exporter = new CsvExporter();
    }else if(formatName == QObject::tr("HTML")){
        exporter = new HtmlExporter();
        exporter->quoteColumnHeaders = false;
        exporter->stringQuoting = "";
        exporter->numberQuoting = "";
    }else if(formatName == QObject::tr("XML")){
        exporter = new XmlExporter();
        exporter->quoteColumnHeaders = false;
        exporter->stringQuoting = "";
        exporter->numberQuoting = "";
    }else{
        Q_ASSERT(false);
        exporter = 0;
    }

    return exporter;
}
