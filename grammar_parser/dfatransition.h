#ifndef DFATRANSITION_H
#define DFATRANSITION_H

#include <QList>

class DFAState;
class DFAItem;
class BNFRuleItem;

class DFATransition
{
public:
    DFATransition();

    //void addSourceDFAItem(DFAItem *item);

    //QList<DFAItem*> sourceDFAItems;
    BNFRuleItem *currentRuleItem;
    DFAState *state;
};

#endif // DFATRANSITION_H
