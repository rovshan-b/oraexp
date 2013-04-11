#include "excelexporter.h"
#include <QFile>
#include <QTemporaryFile>
#include "util/strutil.h"
#include "util/filesystemutil.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include <QDebug>

ExcelExporter::ExcelExporter() :
    zip(0),
    worksheetFile(0),
    tmpFile(0),
    tmpFileStream(0),
    rowCount(0)
{
    this->isXmlFile=true;
}

ExcelExporter::~ExcelExporter()
{
    qDebug("deleting ExcelExporter");

    reset();
}

void ExcelExporter::reset()
{
    if(tmpFileStream){
        delete tmpFileStream;
        tmpFileStream = 0;
    }

    if(tmpFile){
        delete tmpFile;
        tmpFile = 0;
    }

    if(worksheetFile){
        worksheetFile->close();
        delete worksheetFile;
        worksheetFile = 0;
    }

    if(zip){
        zip->close();
        delete zip;
        zip = 0;
    }

    excelColumnTitles.clear();
    rowCount = 0;
}

void ExcelExporter::startDocument(QTextStream &out)
{
    out << QString("<?xml version=\"1.0\" encoding=\"%1\" standalone=\"yes\"?>").arg((QString)out.codec()->name());
    out << "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" "
               "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">";

    createExcelColumnTitles();
    exportColumnDefinitions();

    *tmpFileStream << "<sheetData>";

}

void ExcelExporter::endDocument(QTextStream &out)
{
    *tmpFileStream << "</sheetData>";

    out << QString("<dimension ref=\"A1:%1%2\" />").
           arg(excelColumnTitles.at(excelColumnTitles.size()-1)).
           arg(rowCount);


    out << "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews>";

    out << "<sheetFormatPr defaultRowHeight=\"15\" />";

    copyTmpFile();


    /*
    out << "<printOptions headings=\"false\" gridLines=\"false\" gridLinesSet=\"true\" horizontalCentered=\"false\" "
           "verticalCentered=\"false\" />"
           "<pageMargins left=\"0.7875\" right=\"0.7875\" top=\"1.05277777777778\" bottom=\"1.05277777777778\" "
           "header=\"0.7875\" footer=\"0.7875\" />"
           "<pageSetup blackAndWhite=\"false\" cellComments=\"none\" copies=\"1\" draft=\"false\" firstPageNumber=\"1\" "
           "fitToHeight=\"1\" fitToWidth=\"1\" horizontalDpi=\"300\" orientation=\"portrait\" "
           "pageOrder=\"downThenOver\" paperSize=\"1\" scale=\"100\" useFirstPageNumber=\"true\" "
           "usePrinterDefaults=\"false\" verticalDpi=\"300\" />"
           "<headerFooter differentFirst=\"false\" differentOddEven=\"false\">"
             "<oddHeader>&amp;C&amp;\"Times New Roman,Regular\"&amp;12&amp;A</oddHeader>"
             "<oddFooter>&amp;C&amp;\"Times New Roman,Regular\"&amp;12Page &amp;P</oddFooter>"
           "</headerFooter>"
         "</worksheet>";*/

    out << "</worksheet>";
}

void ExcelExporter::exportColumnHeaders(const QStringList &headers, int from, int /*to*/, QTextStream &/*out*/)
{
    ++rowCount;

    *tmpFileStream << QString("<row customFormat=\"1\" r=\"1\" s=\"1\" spans=\"1:%1\">").arg(excelColumnTitles.size());
    for(int i=0; i<excelColumnTitles.size(); ++i){
        *tmpFileStream << QString("<c r=\"%1%2\" s=\"1\" t=\"inlineStr\"><is><t>%3</t></is></c>").
                         arg(excelColumnTitles.at(i)).arg(1).arg(headers.at(from+i));
    }
    *tmpFileStream << "</row>";
}

void ExcelExporter::exportRow(const QStringList &oneRow, int rowIx, QTextStream &/*out*/)
{
    ++rowCount;
    int excelRowOffset = this->includeColumnHeaders ? 2 : 1;

    *tmpFileStream << QString("<row r=\"%1\" spans=\"1:%2\">").arg(rowIx+excelRowOffset).arg(excelColumnTitles.size());

    for(int i=0; i<oneRow.size(); ++i){
        *tmpFileStream << QString("<c r=\"%1%2\" t=\"inlineStr\"><is><t>%3</t></is></c>").
                         arg(excelColumnTitles.at(i)).arg(rowIx+excelRowOffset).arg(oneRow.at(i));
    }
    *tmpFileStream << "</row>";
}

QTextStream *ExcelExporter::createOutputStream(QString &errorMessage)
{   
    bool couldCopy = FileSystemUtil::copyFile(":/misc/xlsx_template.xlsx",
                                              this->filename,
                                              &errorMessage);
    if(!couldCopy){
        errorMessage = QObject::tr("Could not create file for writing: %1").arg(errorMessage);
        return 0;
    }

    //open zip file for creating sheet in it
    zip = new QuaZip(this->filename);
    bool couldOpen = zip->open(QuaZip::mdAdd);
    if(!couldOpen){
        errorMessage = QObject::tr("Could not open file for writing");
        return 0;
    }
    worksheetFile = new QuaZipFile(zip);
    QuaZipNewInfo worksheetFileInfo("xl/worksheets/sheet1.xml");
    couldOpen = worksheetFile->open(QIODevice::WriteOnly, worksheetFileInfo);
    if(!couldOpen){
        errorMessage = QObject::tr("Could not create worksheet");
        return 0;
    }
    textStream = new QTextStream(worksheetFile);
    setTextStreamProperties();

    //create temporary file for writing data of sheet
    tmpFile = new QTemporaryFile();
    if(!tmpFile->open()){
        errorMessage = tmpFile->errorString();
        return 0;
    }
    tmpFileStream = new QTextStream(tmpFile);
    tmpFileStream->setCodec(this->encoding.toStdString().c_str());

    return textStream;
}

void ExcelExporter::createExcelColumnTitles()
{
    for(int i=this->startColumn; i<=this->endColumn; ++i){
        excelColumnTitles.append(convertToCellTitle(i-this->startColumn));
    }

    qDebug() << "Excel column titles:" << excelColumnTitles;
}

void ExcelExporter::exportColumnDefinitions()
{
    *tmpFileStream << "<cols>";
    for(int i=0; i<excelColumnTitles.size(); ++i){
        *tmpFileStream << QString("<col min=\"%1\" max=\"%1\" width=\"17\" bestFit=\"1\" customWidth=\"1\"/>").arg(i+1);
    }
    *tmpFileStream << "</cols>";
}

void ExcelExporter::copyTmpFile()
{
    tmpFileStream->flush();
    tmpFileStream->seek(0);

    QString buf;
    while(!tmpFileStream->atEnd()){
        buf = tmpFileStream->read(4096);
        if(!buf.isEmpty()){
            *textStream << buf;
        }
    }
}

QString ExcelExporter::convertToCellTitle(int number)
{
    //cell titles are letters. so we will convert number to base 26
    int quotient = number;
    const int base = 26;
    const int asciiA = 65;

    QString result;

    int pos=0;
    while(quotient >= base){
        int divisionResult = quotient / base;
        int remainder = quotient % base;

        result.prepend(QChar(asciiA+remainder));

        quotient = divisionResult;

        ++pos;
    }

    if(pos>0){
        quotient-=1;
    }
    result.prepend(QChar(asciiA+quotient));

    return result;
}
