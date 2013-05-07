#ifndef CSVIMPORTER_H
#define CSVIMPORTER_H

#include <QString>
#include <QStringList>

class QFile;
class QTextStream;
class TextStreamReader;

class CsvImporter
{
public:
    CsvImporter();
    ~CsvImporter();

    enum HeaderOption
    {
        NoHeader,
        BeforeSkip,
        AfterSkip
    };

    bool setFilename(const QString &filename);
    void setEncoding(const QString &encoding);
    void setDelimiter(const QString &delimiter);
    void setSkipRows(int skipRows);
    void setHeaderOption(HeaderOption headerOption);
    void setEnclosures(const QStringList &enclosures);

    void resetPosition();

    bool isEOF() const;

    QStringList readValues();
private:
    QString filename;
    QString encoding;
    QString delimiter;
    int skipRows;
    HeaderOption headerOption;
    QStringList enclosures;

    QFile *file;
    QTextStream *textStream;

    QString readNextField(bool *readEndOfLine);
    void cleanup();

    QString readString(int count);
    QChar readChar();

    QString buffer;

    int maxEnclosureLength;

};

#endif // CSVIMPORTER_H
