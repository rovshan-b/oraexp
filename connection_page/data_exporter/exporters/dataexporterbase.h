#ifndef DATAEXPORTERBASE_H
#define DATAEXPORTERBASE_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QPair>

class ResultsetColumnMetadata;

class DataExporterBase
{
public:
    DataExporterBase();
    virtual ~DataExporterBase();

    QString filename;
    QString encoding;
    bool bom;

    int startRow;
    int startColumn;
    int endRow;
    int endColumn;

    bool includeNullText;
    bool includeColumnHeaders;
    bool quoteColumnHeaders;

    QString stringQuoting;
    QString numberQuoting;

    QString lineEnding;
    QString columnDelimiter;
    bool delimiterAfterLastColumn;

    QList< QPair<QString,QString>* > stringReplacements;

    void prepareColumnHeaders(QStringList &headers);
    void prepareRow(QStringList &row, ResultsetColumnMetadata *columnMetadata);

    virtual void startDocument(){}
    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, QTextStream &out) = 0;
    virtual void endDocument(){}

};

#endif // DATAEXPORTERBASE_H
