#ifndef PARSINGTABLEROW_H
#define PARSINGTABLEROW_H

#include "parsingtableaction.h"
#include <QHash>

class DFA;

class ParsingTableRow
{
public:
    explicit ParsingTableRow();
    ~ParsingTableRow();

    int stateId;

    bool hasEqualActions(ParsingTableRow *other) const;

    bool ownsActions;

    void addAction(int terminalOrKeywordId, ParsingTableAction::ActionType actionType, int stateOrRuleId, int symbolCount=-1);
    void addGoto(int ruleDefId, int stateId);

    void replaceActionsWith(ParsingTableRow *replaceWithRow);


    //terminal,action
    QHash<int, ParsingTableAction* > *actions;
    //rule,target state id
    QHash<int,int> gotos;

    //bool hasAction(int terminalOrKeywordId, ParsingTableAction::ActionType actionType, int stateOrRuleId, int symbolCount) const;
    void removeAction();

};

#endif // PARSINGTABLEROW_H
