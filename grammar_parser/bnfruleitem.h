#ifndef BNFRuleITEM_H
#define BNFRuleITEM_H

#include "ebnftoken.h"

class BNFRule;

class BNFRuleItem
{
public:
    BNFRuleItem();

    bool operator==(const BNFRuleItem &other) const;

    bool isTerminal;
    QString pointsTo;

    EBNFToken token;
    EBNFToken rangeEndToken;

    bool isNegated;

    bool isEpsilon() const;
};

#endif // BNFRuleITEM_H
