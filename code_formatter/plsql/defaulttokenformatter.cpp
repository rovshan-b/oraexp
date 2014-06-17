#include "defaulttokenformatter.h"
#include "code_parser/codescanner.h"

DefaultTokenFormatter::DefaultTokenFormatter()
{
}

QString DefaultTokenFormatter::format(CodeScanner *scanner)
{
    return QString(" %1 ").arg(scanner->getTokenLexeme());
}
