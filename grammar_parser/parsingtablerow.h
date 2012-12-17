#ifndef PARSINGTABLEROW_H
#define PARSINGTABLEROW_H

#include "parsingtableaction.h"
#include <QHash>

class ParsingTableRow
{
public:
    explicit ParsingTableRow();
    ~ParsingTableRow();

    int stateId;
    //terminal,action
    QHash<int,ParsingTableAction*> *actions;
    //rule,target state id
    QHash<int,int> gotos;

    bool hasEqualActions(ParsingTableRow *other) const;

    bool ownsActions;
};

#endif // PARSINGTABLEROW_H
