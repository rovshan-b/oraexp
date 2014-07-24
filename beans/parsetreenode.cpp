#include "parsetreenode.h"
#include "parsetreenodescope.h"
#include "util/parsetreenodeutil.h"

ParseTreeNode::ParseTreeNode() : parentNode(0), tokenInfo(0), symbolCount(0), scope(0), ownsScope(false)
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

ParseTreeNode *ParseTreeNode::findChildForPosition(int position, bool topLevel)
{
    return ParseTreeNodeUtil::findChildForPosition(this, position, topLevel);
}
