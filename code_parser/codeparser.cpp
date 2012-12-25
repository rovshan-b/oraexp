#include "codeparser.h"

CodeParser::CodeParser() : scanner(scanner)
{
}

CodeParser::~CodeParser()
{
    delete scanner;
}
