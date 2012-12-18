#ifndef DFAITEM_H
#define DFAITEM_H

#include <QtGlobal>
#include <QList>
#include "ebnftoken.h"

class BNFRule;
class BNFRuleItem;

class DFAItem
{
public:
    DFAItem();

    bool operator==(const DFAItem &other) const;

    QString toString(bool prependNewLine=true) const;

    BNFRule *rule;

    int altIx; //index into rule->alternatives
    int position; //distinguished position of DFA item

    bool isInitialItem() const;
    bool isCompleteItem() const;
    bool isKernelItem() const;
    int getSymbolCount() const;

    BNFRuleItem *currentRuleItem() const;
    bool pointsTo(const QString &bnfRuleName) const;
};

#endif // DFAITEM_H
