#include "parsetreenode.h"
#include "parsetreenodescope.h"

ParseTreeNode::ParseTreeNode() : tokenInfo(0), symbolCount(0), scope(0), ownsScope(false)
{
}

ParseTreeNode::~ParseTreeNode()
{
    qDeleteAll(children);

    delete tokenInfo;

    if(ownsScope){
        delete scope;
    }
}
