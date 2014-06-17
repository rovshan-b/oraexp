#include "plsqlformatter.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "selectstatementformatter.h"
#include "defaulttokenformatter.h"
#include <QScopedPointer>

PlSqlFormatter::PlSqlFormatter()
{
}

QString PlSqlFormatter::format(const QString &code)
{
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(code)));

    QString result;

    const static int selectKeyword = PlSqlParsingTable::getInstance()->getKeywordIx("SELECT");

    int token;

    do{
        token = scanner->getNextToken(false);

        if(token == selectKeyword){
            result.append(SelectStatementFormatter::format(scanner.data()));
        }else{
            result.append(DefaultTokenFormatter::format(scanner.data()));
        }

    }while(token != PLS_E_O_F);

    return result;
}

bool PlSqlFormatter::checkPrevToken(const QList<int> &prevTokenList, int token)
{
    return (prevTokenList.size() > 0 && prevTokenList[prevTokenList.size()-1] == token);
}

bool PlSqlFormatter::isPrevKeyword(const QList<int> &prevTokenList)
{
    int size = prevTokenList.size();
    if(size == 0){
        return false;
    }

    int prevToken = prevTokenList[size-1];

    if(prevToken < NON_LITERAL_START_IX && PlSqlParsingTable::getInstance()->getKeywordLexeme(prevToken).length() >= MIN_KEYWORD_LENGTH){
        return true;
    }

    return false;
}
