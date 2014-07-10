#ifndef IREDUCELISTENER_H
#define IREDUCELISTENER_H

#include <QStringList>
#include <QStack>

class ParsingTable;
class TokenInfo;

class IReduceListener
{
public:
    virtual void reduced(TokenInfo* ruleInfo, int symbolCount, const QList<TokenInfo*> &reducedTokens, ParsingTable *parsingTable)=0;
    virtual void accepted(ParsingTable *parsingTable){Q_UNUSED(parsingTable);}

    virtual void error(ParsingTable *parsingTable, QStack<TokenInfo*> &tokenStack){Q_UNUSED(parsingTable); Q_UNUSED(tokenStack);}
};

#endif // IREDUCELISTENER_H
