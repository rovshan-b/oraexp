#include "parsetreenode.h"
#include "tokeninfo.h"

ParseTreeNode::ParseTreeNode() : tokenInfo(0), symbolCount(0)
{
}

ParseTreeNode::~ParseTreeNode()
{
    qDeleteAll(children);

    delete tokenInfo;
}
