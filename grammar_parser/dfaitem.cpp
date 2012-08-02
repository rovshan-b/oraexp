#include "dfaitem.h"
#include "bnfrule.h"
#include "bnfruleitem.h"

DFAItem::DFAItem()
{
}

bool DFAItem::operator==(const DFAItem &other) const
{
    if(this->rule!=other.rule ||
            this->altIx!=other.altIx ||
            this->position!=other.position){
        return false;
    }

    return true;
}

bool DFAItem::isInitialItem() const
{
    return position==0;
}

bool DFAItem::isCompleteItem() const
{
    return position==rule->alternatives.at(altIx).size();
}

bool DFAItem::isKernelItem() const
{
    return position>0;
}

BNFRuleItem *DFAItem::currentRuleItem() const
{
    QList < BNFRuleItem * > items = rule->alternatives.at(altIx);
    if(position == items.size()){
        return 0;
    }

    return items.at(position);
}

bool DFAItem::pointsTo(const QString &bnfRuleName) const
{
    return  this->rule->ruleName==bnfRuleName;
}

QString DFAItem::toString() const
{
    QString str;

    const DFAItem *item=this;
    BNFRule *rule=item->rule;
    BNFRuleItem *ruleItem;
    QList <BNFRuleItem *> ruleItems=rule->alternatives.at(item->altIx);
    str.append("\n");
    str.append(rule->ruleName).append(" :");
    for(int k=0; k<ruleItems.size(); ++k){
        ruleItem=ruleItems.at(k);
        str.append(" ");
        if(item->position==k){
            str.append("_ ");
        }
        if(ruleItem->isTerminal){
            str.append("'");
            str.append(ruleItem->token.tokenType==EBNFToken::EPSILON ? "Epsilon" : ruleItem->token.lexeme);
            str.append("'");
        }else{
            str.append(ruleItem->pointsTo);
        }
    }

    if(item->position==ruleItems.size()){
        str.append(" _ ");
    }

    return str;
}
