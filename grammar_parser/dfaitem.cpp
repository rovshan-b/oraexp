#include "dfaitem.h"
#include "bnfrule.h"
#include "bnfruleitem.h"

DFAItem::DFAItem() : rule(0)
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
    return position>0 || rule->isStartRule;
}

int DFAItem::getSymbolCount() const
{
    QList < BNFRuleItem * > items=rule->alternatives.at(altIx);
    /*if(items.size()==1 && items.at(0)->isEpsilon()){
        return 0;
    }*/
    return items.size();
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

QString DFAItem::toString(bool prependNewLine) const
{
    QString str;

    const DFAItem *item=this;
    BNFRule *rule=item->rule;
    BNFRuleItem *ruleItem;
    QList <BNFRuleItem *> ruleItems=rule->alternatives.at(item->altIx);
    if(prependNewLine){
        str.append("\n");
    }
    //std::size_t address = reinterpret_cast<std::size_t>(this);
    //str.append("(").append(QString::number(address)).append(") ");
    str.append(rule->ruleName).append(" :");
    for(int k=0; k<ruleItems.size(); ++k){
        ruleItem=ruleItems.at(k);
        str.append(" ");
        if(item->position==k){
            str.append("_ ");
        }
        if(ruleItem->isTerminal){
            if(ruleItem->token.isLiteralTerminal){str.append("'");}
            str.append(ruleItem->token.tokenType==EBNFToken::EPSILON ? "Epsilon" : ruleItem->token.lexeme);
            if(ruleItem->token.isLiteralTerminal){str.append("'");}
        }else{
            str.append(ruleItem->pointsTo);
        }
    }

    if(item->position==ruleItems.size()){
        str.append(" _ ");
    }

    return str;
}
