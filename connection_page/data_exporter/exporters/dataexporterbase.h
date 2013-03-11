#ifndef DATAEXPORTERBASE_H
#define DATAEXPORTERBASE_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QPair>
#include <QSharedPointer>

class ResultsetColumnMetadata;
class QFile;

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

    bool isXmlFile;

    QList< QPair<QString,QString>* > stringReplacements;

    void prepareColumnHeaders(QStringList &headers);
    void prepareRow(QStringList &row);

    virtual void startDocument(QTextStream &out){Q_UNUSED(out);}
    virtual void exportColumnHeaders(const QStringList &headers, int from, int to, QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out) = 0;
    virtual void endDocument(QTextStream &out){Q_UNUSED(out);}

    virtual QTextStream *createOutputStream(QString &errorMessage);

    QSharedPointer<ResultsetColumnMetadata> columnMetadata;

protected:
    void setTextStreamProperties();
    QTextStream *textStream;

    virtual void prepareField(QString &fieldValue, int fieldIx);

private:
    QFile *file;

};

#endif // DATAEXPORTERBASE_H
