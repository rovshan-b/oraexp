#ifndef PLSQLPARSER_H
#define PLSQLPARSER_H

#include "../codeparser.h"
#include "plsqlscanner.h"

class PlSqlParser : public CodeParser
{
public:
    PlSqlParser(CodeScanner *scanner);

    virtual ParsingTable *getParsingTable();

protected:
    virtual void correctError(int *token, ParsingTableRow *row, ParsingTableAction **actionOnCurrentToken);

private:
    void replaceKeywordWithIdentifier(int token, ParsingTableRow *row, ParsingTableAction **actionOnCurrentToken);
    int reduceMajorConstruct(QList<TokenInfo *> &reduceTokens, int majorConstructs[], int majorConstructCount);
    //bool restoreParsingFromNextState(int *token, QList<TokenInfo *> &reduceTokens);
    //int findStateWithAtionOnToken(int token, ParsingTableRow *row, ParsingTable *table) const;

    //to prevent infinite recursion
    int lastErrorPosition;
    int countOnLastPosition;
};

#endif // PLSQLPARSER_H
