#include "dfatransition.h"
#include "dfaitem.h"
#include "dfastate.h"
#include "bnfruleitem.h"
#include <QString>

DFATransition::DFATransition(DFAItem *sourceItem, DFAState *targetState) : sourceItem(sourceItem), targetState(targetState)
{
}

QString DFATransition::toString(bool prependNewLine) const
{
    QString result;

    QString sourceItemDesc = sourceItem->toString(prependNewLine);
    BNFRuleItem *currentRuleItem=sourceItem->currentRuleItem();
    result.append(sourceItemDesc);
    if(prependNewLine){
        result.append(QString(qMax(10, 80-sourceItemDesc.size()), ' '));
    }else{
        result.append("   ");
    }
    if(currentRuleItem->isTerminal){result.append("'");}
    result.append(currentRuleItem->token.lexeme);
    if(currentRuleItem->isTerminal){result.append("'");}
    result.append(" > ").
            append(QString::number(targetState->stateId));

    return result;
}

//void DFATransition::addSourceDFAItem(DFAItem *item)
//{
//    if(!sourceDFAItems.contains(item)){
//        sourceDFAItems.append(item);
//    }
//}
