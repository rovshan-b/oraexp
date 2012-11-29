#ifndef BNFRULE_H
#define BNFRULE_H

#include <QString>
#include <QList>
#include "ebnftoken.h"

class BNFRuleItem;

class BNFRule
{
public:
    BNFRule();
    ~BNFRule();

    QString ruleName;
    QList< QList < BNFRuleItem * >  > alternatives;

    void startAlternatives();
    void addRuleItem(BNFRuleItem *item);

    int subruleCount;
    bool isReachableFromStartSymbol;

    QList<EBNFToken> firstSet;

    QString toString() const;
};

#endif // BNFRULE_H
