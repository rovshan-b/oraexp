#ifndef FIRSTFOLLOWSETCOMPUTER_H
#define FIRSTFOLLOWSETCOMPUTER_H

#include "bnfrule.h"

class FirstFollowSetComputer
{
public:
    FirstFollowSetComputer(const QList<BNFRule *> &bnfRules);

    QList<BNFRule *> bnfRules;

private:
    void computeFirstSets();
    bool addFirstSetFrom(BNFRule *target, BNFRuleItem *sourceRuleItem);
    bool addToFirstSet(BNFRule *target, const EBNFToken &token);

    BNFRule *findRuleByName(const QString &ruleName);

    void printFirstSets();
};

#endif // FIRSTFOLLOWSETCOMPUTER_H
