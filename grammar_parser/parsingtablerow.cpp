#include "parsingtablerow.h"
#include <QDebug>
#include <QStringList>
#include "dfa.h"
#include "bnfrule.h"

ParsingTableRow::ParsingTableRow() : ownsActions(true), actions(new QHash<int,QList< ParsingTableAction*> >())
{
}

ParsingTableRow::~ParsingTableRow()
{
    if(ownsActions){
        clearActions();
    }
}

bool ParsingTableRow::hasEqualActions(ParsingTableRow *other) const
{
    if(actions->size()!=other->actions->size()){
        return false;
    }

    if(actions->keys()!=other->actions->keys()){
        return false;
    }

    QList< QList<ParsingTableAction*> > actionValues=actions->values();
    QList< QList<ParsingTableAction*> > otherActionValues=other->actions->values();

    if(actionValues.size()!=otherActionValues.size()){
        return false;
    }

    for(int i=0; i<actionValues.size(); ++i){
        QList<ParsingTableAction*> actionList=actionValues.at(i);
        QList<ParsingTableAction*> otherActionList=otherActionValues.at(i);

        for(int k=0; k<actionList.size(); ++k){
            ParsingTableAction *action=actionList.at(k);
            ParsingTableAction *otherAction=otherActionList.at(k);

            if(!(*action==*otherAction)){
                return false;
            }
        }
    }

    return true;
}

void ParsingTableRow::addAction(int terminalOrKeywordId, ParsingTableAction::ActionType actionType, int stateOrRuleId, int symbolCount)
{
    //check if already has this action
    bool alreadyHas=hasAction(terminalOrKeywordId, actionType, stateOrRuleId, symbolCount);
    if(alreadyHas){
        return;
    }

    ParsingTableAction *action=new ParsingTableAction();
    action->actionType=actionType;
    action->stateOrRuleId=stateOrRuleId;
    action->symbolCount=symbolCount;

    (*actions)[terminalOrKeywordId].append(action);
}

void ParsingTableRow::addGoto(int ruleDefId, int stateId)
{
    Q_ASSERT(gotos.value(ruleDefId, -1)==-1 || gotos.value(ruleDefId)==stateId);

    if(!gotos.contains(ruleDefId)){
        gotos[ruleDefId]=stateId;
    }
}

void ParsingTableRow::replaceActionsWith(ParsingTableRow *replaceWithRow)
{
    clearActions();
    this->actions=replaceWithRow->actions;
    this->ownsActions=false;
}

void ParsingTableRow::printout(const DFA *dfa, const QStringList *keywords)
{
    ParsingTableRow *row=this;
    qDebug() << "State" << row->stateId;
    qDebug() << "   Actions:";
    QList<int> actionKeys=row->actions->keys();
    for(int k=0; k<actionKeys.size(); ++k){
        QList<ParsingTableAction*> actionList=row->actions->value(actionKeys.at(k));
        if(actionList.size()>1){
            qDebug() << "      next" << actionList.size() << "items are conflicting items";
        }
        for(int m=0; m<actionList.size(); ++m){
            ParsingTableAction* action=actionList.at(m);
            qDebug() << "      on " << keywords->at(actionKeys.at(k))
                     << action->getActionTypeAsString()
                     << action->stateOrRuleId
                     << (action->actionType==ParsingTableAction::Reduce ? qPrintable(QString("(rb %1)").arg(QString::number(action->symbolCount))) :
                                                                          qPrintable(QString("")));
        }
    }

    qDebug() << "   Gotos:";
    QList<int> gotoKeys=row->gotos.keys();
    for(int l=0; l<gotoKeys.size(); ++l){
        int ruleDefId=gotoKeys.at(l);
        int stateId=row->gotos.value(ruleDefId);
        Q_ASSERT(ruleDefId!=-1);
        qDebug() << "      on " << dfa->bnfRules.at(ruleDefId-1)->ruleName << stateId;
    }
}
/*
bool shiftsFirst(const ParsingTableAction *s1, const ParsingTableAction *s2)
{
    return s1->actionType < s2->actionType;
}*/


//make sure shift actions come before reduce actions
void ParsingTableRow::sortActions()
{
    /*
    QList<int> keys = actions->keys();
    for(int i=0; i<keys.size(); ++i){
        QList<ParsingTableAction*> &itemActions=(*actions)[keys.at(i)];
        qSort(itemActions.begin(), itemActions.end(), shiftsFirst);
    }*/
}

bool ParsingTableRow::hasAction(int terminalOrKeywordId, ParsingTableAction::ActionType actionType, int stateOrRuleId, int symbolCount) const
{
    QList<ParsingTableAction*> actionsForTerminal=actions->value(terminalOrKeywordId);
    if(actionsForTerminal.size()==0){
        return false;
    }

    for(int i=0; i<actionsForTerminal.size(); ++i){
        ParsingTableAction* actionForTerminal=actionsForTerminal.at(i);
        if(actionForTerminal->actionType==actionType &&
                actionForTerminal->stateOrRuleId==stateOrRuleId &&
                actionForTerminal->symbolCount==symbolCount){
            return true;
        }
    }

    return false;
}

void ParsingTableRow::clearActions()
{
    QList< QList<ParsingTableAction*> > values=actions->values();
    for(int i=0; i<values.size(); ++i){
        qDeleteAll(values.at(i));
    }
    delete actions;
}
