#ifndef DFATRANSITION_H
#define DFATRANSITION_H

#include <QList>

class DFAState;
class DFAItem;
class BNFRuleItem;

class DFATransition
{
public:
    DFATransition(DFAItem *sourceItem, DFAState *targetState);

    //void addSourceDFAItem(DFAItem *item);

    //QList<DFAItem*> sourceDFAItems;
    DFAItem *sourceItem;
    DFAState *targetState;
};

#endif // DFATRANSITION_H
