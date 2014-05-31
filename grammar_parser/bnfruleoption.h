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

    //skip this item when building code outline tree and keep searching in its child items
    bool skipInGuiTree;

    //use this name to instantiate meta class for handling this node when building code outline tree
    QString guiHandlerName;

    //dislay name in code outline tree
    QString guiDisplayName;

    //load icon name from application resources for displaying in code outline tree
    QString guiIconName;

    //do not add children in code outline tree
    bool guiNoChildren;
};

#endif // BNFRULEOPTION_H
