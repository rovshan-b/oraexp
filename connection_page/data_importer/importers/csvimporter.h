#ifndef CSVIMPORTER_H
#define CSVIMPORTER_H

#include <QString>
#include <QStringList>

class QFile;
class QTextStream;
class TextStreamReader;
class IDataImportListener;

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
    bool setEncoding(const QString &encoding);
    bool setDelimiter(const QString &delimiter);
    bool setSkipRows(int skipRows);
    bool setHeaderOption(HeaderOption headerOption);
    bool setEnclosures(const QStringList &enclosures);
    bool setBackslashAsEscape(bool backslashAsEscape);
    bool setNullIf(const QStringList &nullIf);

    void resetPosition();

    bool isEOF() const;

    QStringList readValues();

    void readRows(IDataImportListener *importListener, int maxCount = -1);
    void stop();

private:
    QString filename;
    QString encoding;
    QString delimiter;
    int skipRows;
    HeaderOption headerOption;
    QStringList enclosures;
    bool backslashAsEscape;
    QStringList nullIf;

    QFile *file;
    QTextStream *textStream;

    QString readNextField(bool *readEndOfLine);
    void cleanup();

    QString readString(int count);
    QChar readChar();

    QString buffer;

    int maxEnclosureLength;

    bool stopped;

};

#endif // CSVIMPORTER_H
