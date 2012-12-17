#include "parsingtablerow.h"

ParsingTableRow::ParsingTableRow() : actions(new QHash<int,ParsingTableAction*>()), ownsActions(true)
{
}

ParsingTableRow::~ParsingTableRow()
{
    if(ownsActions){
        qDeleteAll(actions->values());
        delete actions;
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

    QList<ParsingTableAction*> actionValues=actions->values();
    QList<ParsingTableAction*> otherActionValues=other->actions->values();
    for(int i=0; i<actionValues.size(); ++i){
        ParsingTableAction *action=actionValues.at(i);
        ParsingTableAction *otherAction=otherActionValues.at(i);

        if(!(*action==*otherAction)){
            return false;
        }
    }

    return true;
}
