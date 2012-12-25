#ifndef CODEPARSERFACTORY_H
#define CODEPARSERFACTORY_H

class CodeParser;

#include "itextreader.h"
#include <QString>

class CodeParserFactory
{
public:
    static CodeParser *createParser(const QString &language, ITextReader *textReader);

private:
    CodeParserFactory();
};

#endif // CODEPARSERFACTORY_H
