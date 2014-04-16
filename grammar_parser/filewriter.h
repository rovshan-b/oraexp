#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>

class FileWriter
{
public:
    enum Destination
    {
        Keywords,
        ReservedWords,
        RuleNames,
        ActionDeclarations,
        ActionInitializations,
        States,
        Rules,
        RuleOptions
    };

    static void write(const QString &str, Destination dest);
    static void writeLine(const QString &line, Destination dest);

    static void flushToFiles();

private:
    FileWriter();

    static QString keywordsCode;
    static QString reservedWordsCode;
    static QString ruleNamesCode;
    static QString actionDesclarationsCode;
    static QString actionInitializationsCode;
    static QString statesCode;
    static QString rulesCode;
    static QString ruleOptionsCode;

    static QString *getBuffer(Destination dest);
};

#endif // FILEWRITER_H
