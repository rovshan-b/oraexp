#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>

class FileWriter
{
public:
    enum Destination
    {
        ParsingTable,
        Rules
    };

    static void write(const QString &str, Destination dest = ParsingTable);
    static void writeLine(const QString &line, Destination dest = ParsingTable);

    static void flushToFiles();

private:
    FileWriter();

    static QString parsingTableCode;
    static QString rulesCode;

    static QString *getBuffer(Destination dest);
};

#endif // FILEWRITER_H
