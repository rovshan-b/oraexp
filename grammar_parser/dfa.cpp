#include "dfa.h"
#include "bnfrule.h"
#include "bnfruleitem.h"
#include "dfaitem.h"
#include "dfastate.h"
#include "dfatransition.h"
#include "firstfollowsetcomputer.h"
#include "ebnfscanner.h"
#include "filewriter.h"
#include <QtDebug>
#include <QTime>

DFA::DFA(const QList<BNFRule*> &bnfRules, int eofTokenId) : bnfRules(bnfRules), hasConflicts(false), stateCounter(0), eofTokenId(eofTokenId)
{
    QTime time;
    time.start();

    if(!bnfRules.isEmpty()){
        augmentStartRule();
        printoutTargetParserRules();
        FirstFollowSetComputer(this->bnfRules);
        generateDFAItems();
        constructDFAforLR0();

        //printoutDFA();
        //return;

        if(hasConflicts){
            return;
        }
        constructDFAforLALR1();
        printoutDFA();
        checkForConflicts();
    }

    qDebug() << "constructed LALR(1) DFA in" << time.elapsed() << "ms";
}

DFA::~DFA()
{
    qDeleteAll(states);
    qDeleteAll(dfaItems);
    qDeleteAll(bnfRules);
}

DFAState *DFA::createDFAState()
{
    DFAState *state=new DFAState();
    state->stateId=stateCounter++;
    states.append(state);

    return state;
}

void DFA::augmentStartRule()
{
    BNFRule *currentStartRule=bnfRules.at(0);

    BNFRule *newStartRule=new BNFRule();
    newStartRule->isStartRule=true;
    newStartRule->ruleName=QString("%1_aug").arg(currentStartRule->ruleName);
    newStartRule->startAlternatives();

    BNFRuleItem *item=new BNFRuleItem();
    item->pointsTo=currentStartRule->ruleName;
    item->token.lexeme=currentStartRule->ruleName;
    item->token.tokenType=EBNFToken::ID;
    item->isTerminal=false;

    newStartRule->addRuleItem(item);

    bnfRules.insert(0, newStartRule);
}

void DFA::generateDFAItems()
{
    QTime time;
    time.start();

    int ruleCount=bnfRules.size();
    BNFRule *rule;
    QList < BNFRuleItem * > ruleItems;
    DFAItem *dfaItem;

    for(int i=0; i<ruleCount; ++i){
        rule=bnfRules.at(i);
        for(int k=0; k<rule->alternatives.size(); ++k){

            ruleItems=rule->alternatives.at(k);

            for(int j=0; j<=ruleItems.size(); ++j){
                dfaItem=new DFAItem();
                dfaItem->rule=rule;
                dfaItem->altIx=k;
                dfaItem->position=j;

                dfaItems.append(dfaItem);
            }
        }
    }

    qDebug() << "generated" << dfaItems.size() << " DFA items in" << time.elapsed() << "ms";
}

void DFA::constructDFAforLR0()
{
    canonicalCollection();

    for(int i=0; i<states.size(); ++i){
        computeTransitions(states.at(i));
    }

    checkTransitions();
}

void DFA::computeTransitions(DFAState *state)
{
    for(int i=0; i<state->dfaItems.size(); ++i){
        DFAItem *dfaItem=state->dfaItems.at(i);
        if(dfaItem->isCompleteItem()){
            continue;
        }

        QList<DFAItem*> targetItems=go_to(state->dfaItems, dfaItem->currentRuleItem());
        DFAState *targetState=hasStateWithItems(targetItems);
        Q_ASSERT(targetState);
        DFATransition *transition=new DFATransition(dfaItem, targetState);
        state->addTransition(transition);
    }
}

int qHash(const EBNFToken &token)
{
    return qHash(token.lexeme)+token.tokenType+token.isLiteralTerminal+token.nonLiteralTerminalDefId;
}

void DFA::checkTransitions()
{
    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        QHash<EBNFToken,DFAState*> stateTransitions;
        for(int k=0; k<state->transitions.size(); ++k){
            DFATransition *transition=state->transitions.at(k);

            EBNFToken currentRuleItemToken = transition->sourceItem->currentRuleItem()->token;
            DFAState *targetState=stateTransitions.value(currentRuleItemToken);
            if(targetState==0){
                stateTransitions[currentRuleItemToken]=transition->targetState;
            }else if(targetState!=transition->targetState){
                qDebug() << "State" << state->stateId << "has transitions on" << currentRuleItemToken.toString() << "to different states";
                if(!hasConflicts){hasConflicts=true;}
            }
        }
    }
    if(hasConflicts){
        printoutDFA();
    }
}

QList<DFAItem*> DFA::closure(QList<DFAItem*> items) const
{
    bool changed;

    QList<DFAItem*> results=items;

    DFAItem *dfaItem;
    BNFRuleItem *ruleItem;
    QList<DFAItem*> productions;
    DFAItem *prodDFAItem;

    do{
        changed=false;

        for(int i=0; i<results.size(); ++i){
            dfaItem=results.at(i);
            ruleItem=dfaItem->currentRuleItem();
            if(ruleItem==0 || ruleItem->isTerminal){ //complete item or terminal
                continue;
            }
            productions=findAllInitialDFAItemsForRule(ruleItem->pointsTo);
            for(int k=0; k<productions.size(); ++k){
                prodDFAItem=productions.at(k);

                //if it's epsilon, slide to complete item
                if(!prodDFAItem->isCompleteItem() && prodDFAItem->currentRuleItem()->isEpsilon()){
                    prodDFAItem=findNextDFAItem(prodDFAItem);
                }

                if(!results.contains(prodDFAItem)){
                    results.append(prodDFAItem);
                    changed=true;
                }
            }

        }
    }while(changed);

    return results;
}

QList<DFAItem*> DFA::go_to(QList<DFAItem*> items, BNFRuleItem *ruleItem)
{
    QList<DFAItem*> results;
    DFAItem *item;
    BNFRuleItem *currRuleItem;
    for(int i=0; i<items.size(); ++i){
        item=items.at(i);
        currRuleItem=item->currentRuleItem();
        if(currRuleItem==0 || currRuleItem->isEpsilon()){ //complete item
            continue;
        }

        if(*currRuleItem==*ruleItem){
            results.append(findNextDFAItem(item));
        }
    }

    results=closure(results);

    return results;
}

void DFA::canonicalCollection()
{
    QList<DFAItem*> firstItem;
    firstItem.append(dfaItems.at(0));
    QList<DFAItem*> cls=closure(firstItem);

    DFAState *state=createDFAState();
    state->dfaItems=cls;

    DFAState *tmpState;
    DFAItem *tmpItem;
    BNFRuleItem *tmpRuleItem;
    QList<DFAItem*> t;
    bool changed;
    do{
        changed=false;

        for(int i=0; i<states.size(); ++i){
            tmpState=states.at(i);
            for(int k=0; k<tmpState->dfaItems.size(); ++k){
                tmpItem=tmpState->dfaItems.at(k);
                tmpRuleItem=tmpItem->currentRuleItem();
                if(tmpRuleItem==0){ //complete item
                    continue;
                }

                t=go_to(tmpState->dfaItems, tmpRuleItem);
                if(!t.isEmpty() && !hasStateWithItems(t)){
                    state=createDFAState();
                    state->dfaItems=t;
                    changed=true;
                }
            }
        }

    }while(changed);

}

void DFA::constructDFAforLALR1()
{
    computeLookaheadPropagations();
    propagateLookaheads();

    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        QList<DFAItem*> keys=state->lookaheads.keys();
        for(int k=0; k<keys.size(); ++k){
            DFAItem *dfaItem=keys.at(k);
            QList<EBNFToken> &itemLookaheads=state->lookaheads[dfaItem];
            for(int j=0; j<itemLookaheads.size(); ++j){
                const EBNFToken &la=itemLookaheads.at(j);
                if(la.tokenType==EBNFToken::E_O_F){
                    itemLookaheads[j].isLiteralTerminal=false;
                    itemLookaheads[j].nonLiteralTerminalDefId=this->eofTokenId;
                }
            }
        }
    }

    closeItems();
    copyLookaheadsToEpsilonItems();
}

void DFA::computeLookaheadPropagations()
{
    int stateCount=states.size();
    for(int i=0; i<stateCount; ++i){
        DFAState *state=states.at(i);
        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *item=state->dfaItems.at(k);
            if(!item->isKernelItem() || item->isCompleteItem()){
                continue;
            }

            DFAState *tmpState=createTmpStateWithNonGrammarSymbol(item);
            closure_lalr1(tmpState);

            setLookaheadPropagations(state, tmpState, item);

            delete tmpState;
        }
    }

    printoutLookaheadsPropagationTable();
}

void DFA::setLookaheadPropagations(DFAState *state, DFAState *tmpState, DFAItem *keyItem)
{
    for(int i=0; i<tmpState->dfaItems.size(); ++i){
        DFAItem *item=tmpState->dfaItems.at(i);
        DFATransition *transition=state->findTransitionOnDFAItem(item);
        if(transition==0){ //complete epsilon item
            continue;
        }
        DFAState *targetState=transition->targetState;
        DFAItem *nextItem=findNextDFAItem(item);
        Q_ASSERT(nextItem);

        QList<EBNFToken> itemLookaheads = tmpState->lookaheads.value(item);
        for(int k=0; k<itemLookaheads.size(); ++k){
            const EBNFToken &lookahead = itemLookaheads.at(k);
            if(lookahead.tokenType==EBNFToken::NON_GRAMMAR){
                lookaheadsPropagationTable[qMakePair(state, keyItem)].append(qMakePair(targetState, nextItem));
            }else{
                targetState->addLookahead(nextItem, lookahead);
            }
        }
    }
}

void DFA::propagateLookaheads()
{
    states.at(0)->addLookahead(dfaItems.at(0), EBNFScanner::createEOFToken());

    bool hasChanges;

    do{
        hasChanges=false;

        QHashIterator<QPair<DFAState*, DFAItem*>,
                      QList< QPair<DFAState*, DFAItem* > > > i(lookaheadsPropagationTable);

        while (i.hasNext()) {
            i.next();

            QList<EBNFToken> sourceLookaheads = i.key().first->lookaheads.value(i.key().second);

            QList< QPair<DFAState*, DFAItem* > > targets = i.value();
            for(int j=0; j<targets.size(); ++j){
                QPair<DFAState*, DFAItem* > target=targets.at(j);
                DFAState *targetState=target.first;
                DFAItem *targetItem=target.second;

                for(int k=0; k<sourceLookaheads.size(); ++k){
                    bool added = targetState->addLookahead(targetItem, sourceLookaheads.at(k));
                    if(!hasChanges && added){hasChanges=true;}
                }
            }
        }
    }while(hasChanges);

    /*
    //copy lookaheads to complete epsilon items
    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *dfaItem=state->dfaItems.at(k);
            if(dfaItem->isCompleteItem() || dfaItem->currentRuleItem()->isTerminal){
                continue;
            }

            QList<DFAItem*> initItems = findAllInitialDFAItemsForRule(dfaItem->currentRuleItem()->pointsTo);
            for(int l=0; l<initItems.size(); ++l){
                DFAItem *initItem = initItems.at(l);
                if(!initItem->currentRuleItem()->isEpsilon()){
                    continue;
                }

                DFAItem *completeEpsilonItem=findNextDFAItem(initItem);
                Q_ASSERT(state->contains(completeEpsilonItem));

                QList<EBNFToken> currentLookaheads=state->lookaheads.value(dfaItem);
                for(int m=0; m<currentLookaheads.size(); ++m){
                    state->addLookahead(completeEpsilonItem, currentLookaheads.at(m));
                }
            }
        }
    }*/
}

void DFA::copyLookaheadsToEpsilonItems()
{
        //copy lookaheads to complete epsilon items
        for(int i=0; i<states.size(); ++i){
            DFAState *state=states.at(i);
            for(int k=0; k<state->dfaItems.size(); ++k){
                DFAItem *dfaItem=state->dfaItems.at(k);
                if(dfaItem->isCompleteItem() || dfaItem->currentRuleItem()->isTerminal){
                    continue;
                }

                DFAItem *epsilonItem = findEpsilonDFAItemForRule(dfaItem->currentRuleItem()->pointsTo);
                if(epsilonItem == 0){
                    continue;
                }

                DFAItem *completeEpsilonItem=findNextDFAItem(epsilonItem);
                if(!state->contains(completeEpsilonItem)){
                    continue;
                }

                QList<DFAItem*> allInitItems = findAllInitialDFAItemsForRule(dfaItem->currentRuleItem()->pointsTo);
                for(int j=0; j<allInitItems.size(); ++j){
                    DFAItem* initItem = allInitItems.at(j);
                    if(initItem->currentRuleItem()->isEpsilon()){
                        continue;
                    }

                    QList<EBNFToken> currentLookaheads=state->lookaheads.value(initItem);
                    for(int m=0; m<currentLookaheads.size(); ++m){
                        state->addLookahead(completeEpsilonItem, currentLookaheads.at(m));
                    }
                }
            }
        }
}

void DFA::closeItems()
{
    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *item=state->dfaItems.at(k);
            if(!item->isKernelItem() || item->isCompleteItem()){
                continue;
            }

            DFAState *tmpState=new DFAState();
            tmpState->addItem(item, true);
            tmpState->lookaheads[item]=state->lookaheads.value(item);
            closure_lalr1(tmpState);

            for(int l=0; l<tmpState->dfaItems.size(); ++l){
                DFAItem *tmpItem=tmpState->dfaItems.at(l);
                if(!tmpItem->isInitialItem()){
                    continue;
                }

                Q_ASSERT(state->contains(tmpItem));
                QList<EBNFToken> lookaheads=tmpState->lookaheads.value(tmpItem);
                //for(int m=0; m<lookaheads.size(); ++m){
                //    const EBNFToken &lookahead=lookaheads.at(m);
                //    state->addLookahead()
                //}
                state->lookaheads[tmpItem]=lookaheads;
            }

            delete tmpState;
        }
    }
}

DFAState *DFA::createTmpStateWithNonGrammarSymbol(DFAItem *item) const
{
    DFAState *state=new DFAState();
    state->addItem(item, true);
    state->addLookahead(item, createNonGrammarToken());

    return state;
}

void DFA::closure_lalr1(DFAState *state) const
{
    bool hasChanges;
    do{
        hasChanges=false;

        for(int i=0; i<state->dfaItems.size(); ++i){
            DFAItem *dfaItem=state->dfaItems.at(i);
            if(dfaItem->isCompleteItem() || dfaItem->currentRuleItem()->isTerminal){
                continue;
            }

            DFAItem *nextDFAItem=findNextDFAItem(dfaItem);
            BNFRuleItem *nextRuleItem=nextDFAItem->currentRuleItem();

            QString lookFor = dfaItem->currentRuleItem()->pointsTo;
            QList<DFAItem*> initItems=findAllInitialDFAItemsForRule(lookFor);
            for(int k=0; k<initItems.size(); ++k){
                DFAItem *initItem=initItems.at(k);
                if(initItem->currentRuleItem()->isEpsilon()){
                    initItem=findNextDFAItem(initItem);
                }

                if(!state->contains(initItem)){
                    state->addItem(initItem, false);
                    hasChanges=true;
                }

                bool nextDfaItemHasEpsilonInFirstSet=false;
                if(!nextDFAItem->isCompleteItem()){
                    if(nextRuleItem->isTerminal && !nextRuleItem->isEpsilon()){
                        bool added=state->addLookahead(initItem, nextRuleItem->token);
                        if(!hasChanges && added){hasChanges=true;}
                    }else if(!nextRuleItem->isTerminal && !nextRuleItem->isEpsilon()){
                        for(int m=0; m<nextDFAItem->rule->firstSet.size(); ++m){
                            const EBNFToken &firstSetToken=nextDFAItem->rule->firstSet.at(m);
                            if(firstSetToken.tokenType!=EBNFToken::EPSILON){
                                bool added=state->addLookahead(initItem, firstSetToken);
                                if(!hasChanges && added){hasChanges=true;}
                            }else{
                                nextDfaItemHasEpsilonInFirstSet=true;
                            }
                        }
                    }
                }else if(nextDFAItem->isCompleteItem() || nextDfaItemHasEpsilonInFirstSet){
                    QList<EBNFToken> currentLookaheads=state->lookaheads.value(dfaItem);
                    for(int m=0; m<currentLookaheads.size(); ++m){
                        const EBNFToken &currentLookahead=currentLookaheads.at(m);
                        Q_ASSERT(currentLookahead.tokenType!=EBNFToken::EPSILON);
                        bool added=state->addLookahead(initItem, currentLookahead);
                        if(!hasChanges && added){hasChanges=true;}
                    }
                }
            }
        }
    }while(hasChanges);

    /*
    //slide all epsilon items to complete items
    for(int i=0; i<state->dfaItems.size(); ++i){
        DFAItem *dfaItem=state->dfaItems.at(i);
        if(!(dfaItem->isInitialItem() && dfaItem->currentRuleItem()->isEpsilon())){
            continue;
        }
        DFAItem *slidedEpsilonItem=findNextDFAItem(dfaItem);
        state->dfaItems.removeOne(dfaItem);
        state->dfaItems.append(slidedEpsilonItem);
        state->lookaheads[slidedEpsilonItem]=state->lookaheads[dfaItem];
        state->lookaheads.remove(dfaItem);
    }*/
}

void DFA::checkForConflicts()
{
    qDebug("-------------Conflicts---------------");

    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);

        for(int k=0; k<state->dfaItems.size(); ++k){
            DFAItem *dfaItem=state->dfaItems.at(k);
            //BNFRuleItem *ruleItem=dfaItem->currentRuleItem();

            for(int j=k+1; j<state->dfaItems.size(); ++j){
                DFAItem *dfaItemToCheck=state->dfaItems.at(j);
                //BNFRuleItem *ruleItemToCheck=dfaItemToCheck->currentRuleItem();

                bool conflictPossible = (hasIntersectingLookaheads(state, dfaItem, dfaItemToCheck));
                if(!conflictPossible){
                    continue;
                }

                if(dfaItem->isCompleteItem() && dfaItemToCheck->isCompleteItem()){ //check for reduce reduce conflict
                    qDebug() << "State -" << state->stateId << "has reduce reduce conflict on items"
                             << dfaItem->toString(false) << "and" << dfaItemToCheck->toString(false);
                    if(!hasConflicts){hasConflicts=true;}
                }
                /*do not print shift reduce conflicts
                else if((dfaItem->isCompleteItem() && !dfaItemToCheck->isCompleteItem() && dfaItemToCheck->currentRuleItem()->isTerminal) ||
                         (!dfaItem->isCompleteItem() && dfaItemToCheck->isCompleteItem()  && dfaItem->currentRuleItem()->isTerminal)){ //shift reduce conflict
                    qDebug() << "State -" << state->stateId << "has shift reduce conflict on items"
                             << dfaItem->toString(false) << "and" << dfaItemToCheck->toString(false);
                }*/
            }
        }
    }

    qDebug("-----------End conflicts-------------");
}

bool DFA::hasIntersectingLookaheads(DFAState *state, DFAItem *first, DFAItem *second) const
{
    QList<EBNFToken> firstLookaheads=state->lookaheads.value(first);
    QList<EBNFToken> secondLookaheads=state->lookaheads.value(second);

    for(int i=0; i<firstLookaheads.size(); ++i){
        if(secondLookaheads.contains(firstLookaheads.at(i))){
            return true;
        }
    }

    return false;
}

QList<DFAItem*> DFA::findAllInitialDFAItemsForRule(const QString &ruleName) const
{
    QList<DFAItem*> results;
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        if(item->pointsTo(ruleName) &&
                item->isInitialItem()){
            results.append(item);
        }
    }
    return results;
}

DFAItem *DFA::findEpsilonDFAItemForRule(const QString &ruleName) const
{
    QList<DFAItem*> initItems = findAllInitialDFAItemsForRule(ruleName);
    for(int i=0; i<initItems.size(); ++i){
        DFAItem *item = initItems.at(i);
        if(item->currentRuleItem()->isEpsilon()){
            return item;
        }
    }

    return 0;
}

DFAItem *DFA::findDFAItem(BNFRule *rule, int altIx, int position) const
{
    DFAItem *item;
    for(int i=0; i<dfaItems.size(); ++i){
        item=dfaItems.at(i);
        if(item->rule==rule && item->altIx==altIx && item->position==position){
            return item;
        }
    }

    return 0;
}

DFAItem *DFA::findNextDFAItem(DFAItem *currItem) const
{
    return findDFAItem(currItem->rule, currItem->altIx, currItem->position+1);
}

DFAState* DFA::hasStateWithItems(QList<DFAItem*> items) const
{
    for(int i=0; i<states.size(); ++i){
        if(states.at(i)->equalsByItems(items)){
            return states.at(i);
        }
    }

    return 0;
}

QList<DFAState *> DFA::findAllStatesWithDFAItem(DFAItem *dfaItem) const
{
    QList<DFAState *> results;

    for(int i=0; i<states.size(); ++i){
        DFAState *state=states.at(i);
        if(state->contains(dfaItem)){
            results.append(state);
        }
    }

    return results;
}

EBNFToken DFA::createNonGrammarToken() const
{
    EBNFToken token;
    token.lexeme="#";
    token.tokenType=EBNFToken::NON_GRAMMAR;

    return token;
}

void DFA::printoutTargetParserRules()
{
    for(int i=0; i<bnfRules.size(); ++i){
        BNFRule *rule=bnfRules.at(i);
        rule->ruleDefId=i+1;

        QString define=QString("#define R_%1 %2").arg(rule->ruleName.toUpper()).arg(i+1);
        FileWriter::writeLine(define, FileWriter::Rules);
    }

    FileWriter::writeLine("//-----------rule string representations-----------------");
    FileWriter::writeLine(QString("ruleNames.reserve( %1 );").arg(bnfRules.size()));
    for(int i=0; i<bnfRules.size(); ++i){
        BNFRule *rule=bnfRules.at(i);

        QString addStmt=QString("ruleNames.append(\"%1\");").arg(rule->ruleName.toUpper());
        FileWriter::writeLine(addStmt);
    }
    FileWriter::writeLine("//-------------------------------------------------------");
}

void DFA::printoutLookaheadsPropagationTable()
{
    qDebug() << "----Lookaheads propagation table----";

    QHashIterator<QPair<DFAState*, DFAItem*>,
                  QList< QPair<DFAState*, DFAItem* > > > i(lookaheadsPropagationTable);
    while (i.hasNext()) {
        i.next();

        qDebug() << "S -" << i.key().first->stateId << " => " << i.key().second->toString(false) << ":";
        QList< QPair<DFAState*, DFAItem* > > targets = i.value();
        for(int i=0; i<targets.size(); ++i){
            QPair<DFAState*, DFAItem* > target=targets.at(i);
            qDebug() << "    S -" << target.first->stateId << " => " << target.second->toString(false);
        }
    }

    qDebug() << "--End lookaheads propagation table--";
}

void DFA::printoutDFA()
{
    qDebug() << "------LALR(1) DFA states (" << states.size() << ")------";
    for(int i=0; i<states.size(); ++i){
        printoutState(states.at(i));
    }
    qDebug() << "-------------------------------------";
}

void DFA::printoutState(DFAState *state)
{
    qDebug() << "---------";
    qDebug() << qPrintable(state->toString());
}
