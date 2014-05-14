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
};

#endif // BNFRULEOPTION_H
