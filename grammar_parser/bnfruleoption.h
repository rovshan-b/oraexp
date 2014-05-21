#ifndef BNFRULEOPTION_H
#define BNFRULEOPTION_H

#include <QString>

class BNFRuleOption
{
public:
    BNFRuleOption();

    //skip this item and add its children
    bool skip;

    //do not add children
    bool noChildren;

    //is scope for declarations
    bool scope;

    //is declaration
    bool symbolTableEntry;
};

#endif // BNFRULEOPTION_H
