#include "dfastate.h"
#include "dfaitem.h"
#include "dfatransition.h"
#include "bnfrule.h"
#include "bnfruleitem.h"

#include <QString>

DFAState::DFAState() : stateId(-1)
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

DFATransition *DFAState::findTransitionOnDFAItem(DFAItem *dfaItem)
{
    for(int i=0; i<transitions.size(); ++i){
        DFATransition *transition=transitions.at(i);
        if(transition->sourceItem==dfaItem){
            return transition;
        }
    }

    return 0;
}

bool DFAState::addLookahead(DFAItem *item, const EBNFToken &lookahead)
{
    QList<EBNFToken> itemLookaheads=lookaheads.value(item);

    for(int i=0; i<itemLookaheads.size(); ++i){
        if(itemLookaheads.at(i)==lookahead){
            return false;
        }
    }

    lookaheads[item].append(lookahead);
    return true;
}

QString DFAState::toString() const
{
    QString str;

    str.append("State ID: ").append(QString::number(stateId));
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        QString itemDesc = item->toString();
        str.append(itemDesc);
        QList<EBNFToken> itemLookaheads=lookaheads.value(item);
        str.append(QString(qMax(10, 50-itemDesc.size()), ' '));
        for(int k=0; k<itemLookaheads.size(); ++k){
            str.append(itemLookaheads.at(k).lexeme);

            if(k!=itemLookaheads.size()-1){
                str.append(" / ");
            }
        }
        //if(item->isKernelItem()){
        //    str.append(" (kernel item)");
        //}
    }

    if(transitions.size()>0){
        str.append("\nTransitions:");
        for(int k=0; k<transitions.size(); ++k){
            DFATransition *trans=transitions.at(k);
            QString sourceItemDesc = trans->sourceItem->toString();
            str.append(sourceItemDesc).
                    append(QString(qMax(10, 50-sourceItemDesc.size()), ' ')).
                    append(trans->sourceItem->currentRuleItem()->token.lexeme).
                    append(" > ").
                    append(QString::number(trans->targetState->stateId));
        }
    }

    return str;
}
