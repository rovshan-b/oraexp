#include "parsingtablerow.h"

ParsingTableRow::ParsingTableRow() : ownsActions(true), actions(new QHash<int, ParsingTableAction* >())
{
}

ParsingTableRow::~ParsingTableRow()
{
    if(ownsActions){
        removeAction();
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

    QList<ParsingTableAction*> actionList=actions->values();
    QList<ParsingTableAction*> otherActionList=other->actions->values();

    for(int k=0; k<actionList.size(); ++k){
        ParsingTableAction *action=actionList.at(k);
        ParsingTableAction *otherAction=otherActionList.at(k);

        if(!(*action==*otherAction)){
            return false;
        }
    }

    return true;
}

void ParsingTableRow::addAction(int terminalOrKeywordId, ParsingTableAction::ActionType actionType, int stateOrRuleId, int symbolCount)
{
    ParsingTableAction *currentAction=actions->value(terminalOrKeywordId, 0);

    if(currentAction!=0 && actionType==ParsingTableAction::Shift && currentAction->actionType==ParsingTableAction::Reduce){
        delete currentAction;
    }else if(currentAction!=0){
        return;
    }

    ParsingTableAction *action=new ParsingTableAction();
    action->actionType=actionType;
    action->stateOrRuleId=stateOrRuleId;
    action->symbolCount=symbolCount;

    (*actions)[terminalOrKeywordId]=action;
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
    removeAction();
    this->actions=replaceWithRow->actions;
    this->ownsActions=false;
}

/*
bool shiftsFirst(const ParsingTableAction *s1, const ParsingTableAction *s2)
{
    return s1->actionType < s2->actionType;
}


//make sure shift actions come before reduce actions for same terminals
void ParsingTableRow::sortActions()
{
    QList<int> keys = actions->keys();
    for(int i=0; i<keys.size(); ++i){
        QList<ParsingTableAction*> &itemActions=(*actions)[keys.at(i)];
        qSort(itemActions.begin(), itemActions.end(), shiftsFirst);
    }
}*/
/*
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
}*/

void ParsingTableRow::removeAction()
{
    qDeleteAll(actions->values());
    delete actions;
}
