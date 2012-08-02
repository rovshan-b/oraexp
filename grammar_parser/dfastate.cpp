#include "dfastate.h"
#include "dfaitem.h"
#include "dfatransition.h"
#include "bnfrule.h"
#include "bnfruleitem.h"

#include <QString>

DFAState::DFAState()
{
}

DFAState::~DFAState()
{
    qDeleteAll(transitions);
}

void DFAState::addItem(DFAItem *item, bool kernelItem)
{
    dfaItems.append(item);
    if(kernelItem){
        kernelItemIndexes.append(dfaItems.size()-1);
    }
}

bool DFAState::contains(DFAItem *item) const
{
    return dfaItems.contains(item);
}

int DFAState::kernelItemCount() const
{
    return kernelItemIndexes.size();
}

bool DFAState::isKernelItem(DFAItem *item) const
{
    return containsKernelItem(item);
}

bool DFAState::containsKernelItem(DFAItem *item) const
{
    return kernelItemIndexes.contains(dfaItems.indexOf(item));
}

bool DFAState::containsKernelItems(QList<DFAItem*> items) const
{
    if(items.size()==0){
        return false;
    }

    for(int i=0; i<items.size(); ++i){
        if(!containsKernelItem(items.at(i))){
            return false;
        }
    }

    return true;
}

bool DFAState::equalsByKernelItems(DFAState *otherState) const
{
    if(this->kernelItemCount()!=otherState->kernelItemCount()){
        return false;
    }

    DFAItem *kernelItem;
    DFAItem *otherKernelItem;
    for(int i=0; i<kernelItemIndexes.size(); ++i){
        kernelItem=dfaItems.at(kernelItemIndexes.at(i));

        bool allFound=true;
        for(int k=0; k<otherState->kernelItemIndexes.size(); ++k){
            otherKernelItem=otherState->dfaItems.at(otherState->kernelItemIndexes.at(k));

            if(kernelItem!=otherKernelItem){
                allFound=false;
                break;
            }
        }

        if(allFound){
            return true;
        }
    }

    return false;
}

void DFAState::addTransition(DFATransition *transition)
{
    transitions.append(transition);
}

bool DFAState::operator==(const DFAState &other) const
{
    if(this->dfaItems.size()!=other.dfaItems.size()){
        return false;
    }

    if(this->kernelItemCount()!=other.kernelItemCount()){
        return false;
    }


    DFAItem *thisItem;
    DFAItem *otherItem;
    for(int i=0; i<this->dfaItems.size(); ++i){
        thisItem=this->dfaItems.at(i);
        otherItem=other.dfaItems.at(i);

        if(thisItem != otherItem){
            return false;
        }

    }

    return true;
}

bool DFAState::equalsByItems(QList<DFAItem*> items) const
{
    if(this->dfaItems.size()!=items.size()){
        return false;
    }

    DFAItem *thisItem;
    for(int i=0; i<this->dfaItems.size(); ++i){
        thisItem=this->dfaItems.at(i);

        if(!items.contains(thisItem)){
            return false;
        }

    }

    return true;
}

DFATransition *DFAState::findTransitionOnRuleItem(BNFRuleItem *ruleItem)
{
    DFATransition *t;
    for(int i=0; i<transitions.size(); ++i){
        t=transitions.at(i);

        if(*ruleItem == *t->currentRuleItem){
            return t;
        }

    }

    return 0;
}

QString DFAState::toString() const
{
    QString str;

    str.append("State ID: ").append(QString::number(stateId));
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        str.append(item->toString());
        if(isKernelItem(item)){
            str.append(" (kernel item)");
        }
    }
/*
    if(transitions.size()>0){
        str.append("\nTransitions:\n");
        DFATransition *transition;
        for(int i=0; i<transitions.size(); ++i){
            transition=transitions.at(i);
            str.append("On ").append(transition->currentRuleItem->isTerminal ?
                                         transition->currentRuleItem->token.lexeme :
                                         transition->currentRuleItem->pointsTo);
            str.append(transition->currentRuleItem->isTerminal ? " shift" : " go");
            //str.append(" to state ").append(QString::number(transition->state->stateId)).append("\n");
        }
    }
*/
    return str;
}
