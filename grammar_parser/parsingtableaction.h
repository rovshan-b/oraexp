#ifndef PARSINGTABLEACTION_H
#define PARSINGTABLEACTION_H

#include <QString>

class ParsingTableAction
{
public:
    enum ActionType
    {
        Shift,
        Reduce,
        Accept
    };

    explicit ParsingTableAction();

    bool operator==(const ParsingTableAction &other) const;

    ActionType actionType;
    int stateOrRuleId;

    int symbolCount; //only for reduce actions for rolling back to correct state

    QString getActionTypeAsString() const;
};

#endif // PARSINGTABLEACTION_H
