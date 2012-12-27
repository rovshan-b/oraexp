#include "codeparser.h"

CodeParser::CodeParser(CodeScanner *scanner) : scanner(scanner)
{
}

CodeParser::~CodeParser()
{
    delete scanner;
}
