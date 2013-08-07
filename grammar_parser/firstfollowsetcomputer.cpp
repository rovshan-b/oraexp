#include "firstfollowsetcomputer.h"
#include "bnfruleitem.h"
#include "ebnfscanner.h"
#include "ebnfparser.h"
#include <QDebug>

FirstFollowSetComputer::FirstFollowSetComputer(const QList<BNFRule*> &bnfRules) :
    bnfRules(bnfRules)
{
    computeFirstSets();
    printFirstSets();

    computeFollowSets();
    printFollowSets();
}

void FirstFollowSetComputer::computeFirstSets()
{
    bool hasChanges=true;
    int k;
    bool doContinue;

    while(hasChanges){
        hasChanges=false;
        for(int i=0; i<bnfRules.size(); ++i){
            BNFRule *rule=bnfRules.at(i);

            for(int j=0; j<rule->alternatives.size(); ++j){
                k=0;
                doContinue=true;

                QList < BNFRuleItem * > production=rule->alternatives.at(j);

                while(doContinue && k<production.size()){
                    //bool hasEpsilon=false;

                    BNFRuleItem *ruleItem=production.at(k);
                    if(!ruleItem->isEpsilon()){
                        bool added=addFirstSetFrom(rule, ruleItem);
                        if(hasChanges==false && added==true){
                            hasChanges=true;
                        }

                        if(ruleItem->isTerminal || !hasEpsilonInFirstSet(ruleItem->pointsTo)){
                            doContinue=false;
                        }
                    }

                    ++k;
                }

                if(doContinue==true){
                    EBNFToken epsilonToken;
                    epsilonToken.tokenType=EBNFToken::EPSILON;
                    bool added=addToFirstSet(rule, epsilonToken);
                    if(hasChanges==false && added==true){
                        hasChanges=true;
                    }
                }
            }
        }
    }
}

bool FirstFollowSetComputer::addFirstSetFrom(BNFRule *target,
                                             BNFRuleItem *sourceRuleItem)
{
    Q_ASSERT(!sourceRuleItem->isEpsilon()); //must not reach this point
    //if(sourceRuleItem->isEpsilon()){
    //    return addToFirstSet(target, sourceRuleItem->token);
    //}

    if(sourceRuleItem->isTerminal){
        return addToFirstSet(target, sourceRuleItem->token);
    }

    BNFRule *sourceRule=findRuleByName(sourceRuleItem->pointsTo);
    Q_ASSERT(sourceRule!=0);

    bool hasChanges=false;
    for(int i=0; i<sourceRule->firstSet.size(); ++i){
        const EBNFToken &token=sourceRule->firstSet.at(i);
        bool added;

        if(token.tokenType == EBNFToken::EPSILON){
            added = false;
        }else{
            added = addToFirstSet(target, token);
        }

        if(hasChanges==false && added==true){
            hasChanges=true;
        }
    }

    return hasChanges;
}

bool FirstFollowSetComputer::addToFirstSet(BNFRule *target, const EBNFToken &token)
{
    for(int i=0; i<target->firstSet.size(); ++i){
        if(target->firstSet.at(i)==token){
            return false; //already contains
        }
    }

    //if we reached here, target first set does not contain token
    target->firstSet.append(token);

    return true;
}

void FirstFollowSetComputer::computeFollowSets()
{
    EBNFToken eofToken=EBNFScanner::createEOFToken();

    bnfRules.at(0)->followSet.append(eofToken);

    bool hasChanges=true;

    while(hasChanges){
        hasChanges=false;

        for(int i=0; i<bnfRules.size(); ++i){
            BNFRule *rule=bnfRules.at(i);

            for(int j=0; j<rule->alternatives.size(); ++j){

                QList < BNFRuleItem * > production=rule->alternatives.at(j);
                //Q_ASSERT(production.size()>0);

                if(production.isEmpty()){ //epsilon production
                    continue;
                }

                BNFRuleItem *currentRuleItem=production.at(production.size()-1);

                if(!currentRuleItem->isTerminal){
                    BNFRule *pointedRule=findRuleByName(currentRuleItem->pointsTo);
                    Q_ASSERT(pointedRule);
                    //copy follow set of production left side to follow set of last symbol
                    for(int l=0; l<rule->followSet.size(); ++l){
                        Q_ASSERT(rule->followSet.at(l).tokenType!=EBNFToken::EPSILON); //follow set cannot contain epsilon
                        bool added=addToFollowSet(pointedRule, rule->followSet.at(l));
                        if(hasChanges==false && added==true){
                            hasChanges = true;
                        }
                    }
                }

                for(int k=production.size()-1; k>=1; --k){
                    currentRuleItem=production.at(k);

                    BNFRuleItem *prevRuleItem=production.at(k-1);
                    if(prevRuleItem->isTerminal){
                        continue;
                    }

                    BNFRule *prevPointedRule=findRuleByName(prevRuleItem->pointsTo);
                    if(currentRuleItem->isTerminal){
                        bool added=addToFollowSet(prevPointedRule, currentRuleItem->token);
                        if(hasChanges==false && added==true){
                            hasChanges = true;
                        }
                        continue;
                    }

                    bool hasEpsilonInFirstSet=false;
                    bool added=addFollowSetFrom(prevPointedRule, currentRuleItem, &hasEpsilonInFirstSet);
                    if(hasChanges==false && added==true){
                        hasChanges = true;
                    }

                    if(hasEpsilonInFirstSet){ //add folow set as well
                        BNFRule *pointedRule=findRuleByName(currentRuleItem->pointsTo);
                        for(int m=0; m<pointedRule->followSet.size(); ++m){
                            bool added=addToFollowSet(prevPointedRule, pointedRule->followSet.at(m));
                            if(hasChanges==false && added==true){
                                hasChanges = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool FirstFollowSetComputer::addFollowSetFrom(BNFRule *target, BNFRuleItem *sourceRuleItem,
                                              bool *hasEpsilonInFirstSet)
{
    if(sourceRuleItem->isTerminal){
        return addToFollowSet(target, sourceRuleItem->token);
    }

    BNFRule *sourceRule=findRuleByName(sourceRuleItem->pointsTo);
    Q_ASSERT(sourceRule!=0);

    bool hasChanges=false;

    for(int i=0; i<sourceRule->firstSet.size(); ++i){
        if(sourceRule->firstSet.at(i).tokenType==EBNFToken::EPSILON){

            if(*hasEpsilonInFirstSet==false){
                *hasEpsilonInFirstSet=true;
            }

            continue;
        }

        bool added=addToFollowSet(target, sourceRule->firstSet.at(i));

        if(hasChanges==false && added==true){
            hasChanges=true;
        }
    }

    return hasChanges;
}

bool FirstFollowSetComputer::addToFollowSet(BNFRule *target, const EBNFToken &token)
{
    for(int i=0; i<target->followSet.size(); ++i){
        if(target->followSet.at(i)==token){
            return false; //already contains
        }
    }

    //if we reached here, target follow set does not contain token
    Q_ASSERT(token.tokenType!=EBNFToken::EPSILON);
    target->followSet.append(token);

    return true;
}

bool FirstFollowSetComputer::hasEpsilonInFirstSet(const QString &ruleName)
{
    BNFRule *rule = findRuleByName(ruleName);
    foreach(const EBNFToken &token, rule->firstSet){
        if(token.tokenType == EBNFToken::EPSILON){
            return true;
        }
    }

    return false;
}

BNFRule *FirstFollowSetComputer::findRuleByName(const QString &ruleName)
{
    return EBNFParser::findRuleByName(this->bnfRules, ruleName);
}

void FirstFollowSetComputer::printFirstSets()
{
    qDebug("---------first sets----------");
    for(int i=0; i<bnfRules.size(); ++i){
        BNFRule *rule=bnfRules.at(i);
        qDebug() << rule->ruleName << " : ";
        for(int j=0; j<rule->firstSet.size(); ++j){
            qDebug() << "   " << rule->firstSet.at(j).toString();
        }
    }
    qDebug("-------end first sets--------");
}

void FirstFollowSetComputer::printFollowSets()
{
    qDebug("---------follow sets----------");
    for(int i=0; i<bnfRules.size(); ++i){
        BNFRule *rule=bnfRules.at(i);
        qDebug() << rule->ruleName << " : ";
        for(int j=0; j<rule->followSet.size(); ++j){
            qDebug() << "   " << rule->followSet.at(j).toString();
        }
    }
    qDebug("-------end follow sets--------");
}
