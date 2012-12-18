#include "parsingtableaction.h"

ParsingTableAction::ParsingTableAction() : stateOrRuleId(-1), symbolCount(-1)
{
}

bool ParsingTableAction::operator ==(const ParsingTableAction &other) const
{
    if(actionType==other.actionType && stateOrRuleId==other.stateOrRuleId && symbolCount==other.symbolCount){
        return true;
    }

    return false;
}

QString ParsingTableAction::getActionTypeAsString() const
{
    QString result;

    switch(this->actionType){
    case Shift:
        result="Shift";
        break;
    case Reduce:
        result="Reduce";
        break;
    case Accept:
        result="Accept";
        break;
    default:
        result="Unknown";
        break;
    }

    return result;
}
