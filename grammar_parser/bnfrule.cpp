#include "bnfrule.h"
#include "ebnftoken.h"
#include "bnfruleitem.h"

BNFRule::BNFRule() : subruleCount(0), isReachableFromStartSymbol(false)
{

}

BNFRule::~BNFRule()
{
    for(int i=0; i<alternatives.size(); ++i){
        QList <BNFRuleItem*> items = alternatives.at(i);
        qDeleteAll(items);
    }

    //qDeleteAll(firstSet);
}

void BNFRule::startAlternatives()
{
    alternatives.append(QList<BNFRuleItem*>());
}

void BNFRule::addRuleItem(BNFRuleItem *item){
    alternatives[alternatives.size()-1].append(item);
}

QString BNFRule::toString() const
{
    QString str;

    BNFRuleItem *item;
    str.append(ruleName).append(" : ");
    for(int i=0; i<alternatives.size(); ++i){
        if(i>0){
            str.append(" | ");
        }
        QList<BNFRuleItem*> items=alternatives.at(i);
        for(int k=0; k<items.size(); ++k){
            item=items.at(k);

            if(item->isTerminal){
                str.append(" ");
                if(item->token.isLiteralTerminal){str.append("'");}
                str.append(item->token.tokenType==EBNFToken::EPSILON ? "Epsilon" : item->token.lexeme);
                if(item->token.isLiteralTerminal){str.append("'");}
            }else{
                str.append(" ");
                if(item->isNegated){
                    str.append("~");
                }
                str.append(item->pointsTo);
            }
        }
    }
    str.append(" ;");

    return str;
}
