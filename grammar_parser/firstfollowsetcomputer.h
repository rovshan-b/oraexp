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

    void computeFollowSets();
    bool addFollowSetFrom(BNFRule *target, BNFRuleItem *sourceRuleItem, bool *hasEpsilonInFirstSet);
    bool addToFollowSet(BNFRule *target, const EBNFToken &token);

    bool hasEpsilonInFirstSet(const QString &ruleName);

    BNFRule *findRuleByName(const QString &ruleName);

    void printFirstSets();
    void printFollowSets();
};

#endif // FIRSTFOLLOWSETCOMPUTER_H
