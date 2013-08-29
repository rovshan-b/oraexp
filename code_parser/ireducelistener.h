#ifndef IREDUCELISTENER_H
#define IREDUCELISTENER_H

#include <QStringList>

class ParsingTable;
class TokenInfo;

class IReduceListener
{
public:
    virtual void reduced(TokenInfo* ruleInfo, int symbolCount, const QList<TokenInfo*> &reducedTokens, ParsingTable *parsingTable)=0;
    virtual void accepted(){}
};

#endif // IREDUCELISTENER_H
