#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>
#include <QStringList>

class FileWriter
{
public:
    enum Destination
    {
        Keywords,
        ReservedWords,
        RuleNames,
        ActionDefinitions,
        Rules,
        RuleOptions
    };

    static void writeLine(const QString &line, Destination dest);

    static void writeActionDeclaration(const QString &line);
    static void writeActionInitialization(const QString &line);
    static void writeState(const QString &line);

    static void flushToFiles();

private:
    FileWriter();

    static void write(const QString &str, Destination dest);

    static QString keywordsCode;
    static QString reservedWordsCode;
    static QString ruleNamesCode;
    static QString actionDefinitionsCode;
    static QStringList actionDesclarationsCode;
    static QStringList actionInitializationsCode;
    static QStringList statesCode;
    static QString rulesCode;
    static QString ruleOptionsCode;

    static QStringList generatedHeaderFiles;
    static QStringList generatedSourceFiles;

    static QString *getBuffer(Destination dest);

    static void flushActionDeclarations();
    static void flushActionInitializations();
    static void flushStateInitializations();

    static void flushGeneratedFileList();
};

#endif // FILEWRITER_H
