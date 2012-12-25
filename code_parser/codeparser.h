#ifndef CODEPARSER_H
#define CODEPARSER_H

#include "codescanner.h"

class CodeParser
{
public:
    CodeParser(CodeScanner *scanner);
    virtual ~CodeParser();

protected:
    CodeScanner *scanner;
};

#endif // CODEPARSER_H
