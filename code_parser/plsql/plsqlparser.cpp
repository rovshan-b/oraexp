#include "plsqlparser.h"
#include "plsqlparsingtable.h"

PlSqlParser::PlSqlParser(CodeScanner *scanner) : CodeParser(scanner)
{
}

ParsingTable *PlSqlParser::getParsingTable()
{
    return PlSqlParsingTable::getInstance();
}
