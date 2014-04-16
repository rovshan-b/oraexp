#ifndef BNFRULE_H
#define BNFRULE_H

#include <QString>
#include <QList>
#include "ebnftoken.h"
#include "bnfruleoption.h"

class BNFRuleItem;

class BNFRule
{
public:
    BNFRule();
    ~BNFRule();

    QString ruleName;
    QList< QList < BNFRuleItem * >  > alternatives;

    bool isStartRule;

    void startAlternatives();
    void addRuleItem(BNFRuleItem *item);

    int subruleCount;
    bool isReachableFromStartSymbol;

    QList<EBNFToken> firstSet;
    QList<EBNFToken> followSet;

    int ruleDefId; //to be used in target parser

    BNFRuleOption *ruleOptions;

    void readOptions(const QString &options);
    QString codeForOptions() const;

    QString toString() const;
};

#endif // BNFRULE_H
