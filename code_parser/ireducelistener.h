#ifndef IREDUCELISTENER_H
#define IREDUCELISTENER_H

#include <QStringList>

class ParsingTable;
class TokenInfo;

class IReduceListener
{
public:
    virtual void reduced(TokenInfo* ruleInfo, int symbolCount, const QList<TokenInfo*> &reducedTokens, ParsingTable *parsingTable)=0;
    virtual void accepted(ParsingTable *parsingTable){Q_UNUSED(parsingTable);}

    virtual void error(ParsingTable *parsingTable){Q_UNUSED(parsingTable);}
};

#endif // IREDUCELISTENER_H
