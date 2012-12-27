#ifndef CODEPARSERFACTORY_H
#define CODEPARSERFACTORY_H

class CodeParser;

#include "textreaderbase.h"
#include <QString>

class CodeParserFactory
{
public:
    static CodeParser *createParser(const QString &language, TextReaderBase *textReader);

private:
    CodeParserFactory();
};

#endif // CODEPARSERFACTORY_H
