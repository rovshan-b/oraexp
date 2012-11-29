#include "firstfollowsetcomputer.h"
#include "bnfruleitem.h"
#include <QDebug>

FirstFollowSetComputer::FirstFollowSetComputer(const QList<BNFRule*> &bnfRules) :
    bnfRules(bnfRules)
{
    computeFirstSets();
    printFirstSets();
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

                QList < BNFRuleItem * > alternative=rule->alternatives.at(j);

                while(doContinue && k<alternative.size()){
                    bool hasEpsilon=false;

                    BNFRuleItem *ruleItem=alternative.at(k);
                    if(!ruleItem->isEpsilon()){
                        bool added=addFirstSetFrom(rule, ruleItem);
                        if(hasChanges==false && added==true){
                            hasChanges=true;
                        }
                    }else{
                        hasEpsilon=true;
                    }

                    if(hasEpsilon==false){
                        doContinue=false;
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
        bool added=addToFirstSet(target, token);

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

BNFRule *FirstFollowSetComputer::findRuleByName(const QString &ruleName)
{
    for(int i=0; i<bnfRules.size(); ++i){
        if(ruleName==bnfRules.at(i)->ruleName){
            return bnfRules.at(i);
        }
    }

    return 0;
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
