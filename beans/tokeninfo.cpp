#include "tokeninfo.h"
#include "code_parser/plsql/plsqlparsingtable.h"

TokenInfo::TokenInfo() :
    tokenOrRuleId(-1),
    startPos(-1),
    endPos(-1),
    startLine(-1),
    endLine(-1),
    startLinePos(-1),
    endLinePos(-1)
{
}

QString TokenInfo::toString() const
{
    QString result;

    result.append("Type = ").append(tokenType == Token ? "token, " : "rule, ");
    if(tokenType == Token){
        result.append("Lexeme = ").append(lexeme).append(", ");
    }else{
        result.append("Name = ").append(PlSqlParsingTable::getInstance()->getRuleName(tokenOrRuleId)).append(", ");
    }
    result.append("Positions = (").append(QString::number(startPos)).append(", ").append(QString::number(endPos)).append(")");

    return result;
}
