#include "codeparserfactory.h"
#include "plsql/plsqlscanner.h"
#include "plsql/plsqlparser.h"

CodeParserFactory::CodeParserFactory()
{
}

CodeParser *CodeParserFactory::createParser(const QString &language, TextReaderBase *textReader)
{
    CodeParser *parser=0;

    if(language=="plsql"){
        PlSqlScanner *scanner=new PlSqlScanner(textReader);
        parser = new PlSqlParser(scanner);
    }else{
        Q_ASSERT(false);
    }

    return parser;
}
