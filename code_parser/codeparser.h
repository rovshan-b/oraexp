#ifndef CODEPARSER_H
#define CODEPARSER_H

#include "codescanner.h"
#include "parsingtable.h"
#include <QStack>
#include <QPair>

class ParsingTable;
class ParsingTableRow;
class IReduceListener;
class TokenInfo;

class CodeParser
{
public:
    CodeParser(CodeScanner *scanner);
    virtual ~CodeParser();

    virtual ParsingTable *getParsingTable()=0;

    bool parse();

    //first list is for terminals, second for rules
    QPair< QList<int>, QList<int> > getExpectedTokens(ParsingTableRow *row = 0) const;

    CodeScanner *getScanner() const {return this->scanner;}

    void setStrictMode(bool strict);

    void setReduceListener(IReduceListener *listener) {this->reduceListener=listener;}

    void stop(){this->stopped = true;}

protected:
    virtual void correctError(int *token, ParsingTableRow *row, ParsingTableAction **actionOnCurrentToken) = 0;
    void shift(int stateId, int *token);
    void reduce(int ruleId, int symbolCount, const QList<TokenInfo*> &additionalReduceTokens = QList<TokenInfo*>());
    TokenInfo *createTokenInfo(int token) const;

    CodeScanner *scanner;
    ParsingTable *parsingTable;

    QStack<int> stack;
    QStack<TokenInfo*> tokenStack;

    ParsingTableRow *errorRow;

    bool strict;

private:
    IReduceListener *reduceListener;

    bool stopped;
};

#endif // CODEPARSER_H
