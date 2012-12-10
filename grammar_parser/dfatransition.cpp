#include "dfatransition.h"

DFATransition::DFATransition(DFAItem *sourceItem, DFAState *targetState) : sourceItem(sourceItem), targetState(targetState)
{
}

//void DFATransition::addSourceDFAItem(DFAItem *item)
//{
//    if(!sourceDFAItems.contains(item)){
//        sourceDFAItems.append(item);
//    }
//}
