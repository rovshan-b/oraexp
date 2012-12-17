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

    ParsingTableAction();

    bool operator==(const ParsingTableAction &other) const;

    ActionType actionType;
    int stateOrRuleId;

    QString getActionTypeAsString() const;
};

#endif // PARSINGTABLEACTION_H
