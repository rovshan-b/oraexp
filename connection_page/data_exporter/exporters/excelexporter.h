#ifndef EXCELEXPORTER_H
#define EXCELEXPORTER_H

#include "dataexporterbase.h"

class QuaZip;
class QuaZipFile;
class QTemporaryFile;

class ExcelExporter : public DataExporterBase
{
public:
    ExcelExporter();
    virtual ~ExcelExporter();

    virtual void reset();

    virtual void startDocument(QTextStream &out);
    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out);
    virtual void endDocument(QTextStream &out);

    virtual QString getMimeType() const {return "application/vnd.ms-excel";}

    virtual QTextStream *createOutputStream(QString &errorMessage);

private:
    QuaZip *zip;
    QuaZipFile *worksheetFile;
    QTemporaryFile *tmpFile;
    QTextStream *tmpFileStream;

    QStringList excelColumnTitles;

    int rowCount;

    void createExcelColumnTitles();
    void exportColumnDefinitions();
    void copyTmpFile();

    QString convertToCellTitle(int number);
};

#endif // EXCELEXPORTER_H
