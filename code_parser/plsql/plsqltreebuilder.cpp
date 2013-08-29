#include "plsqltreebuilder.h"
#include "beans/tokeninfo.h"
#include "beans/parsetreenode.h"

PlSqlTreeBuilder::PlSqlTreeBuilder() : rootNode(0)
{
}

PlSqlTreeBuilder::~PlSqlTreeBuilder()
{
    delete rootNode;
}

void PlSqlTreeBuilder::reduced(TokenInfo *ruleInfo, int symbolCount, const QList<TokenInfo *> &reducedTokens, ParsingTable *parsingTable)
{
    ParseTreeNode *newNode = new ParseTreeNode();
    newNode->tokenInfo = ruleInfo;
    newNode->symbolCount = symbolCount;

    TokenInfo *ti;
    for(int i=0; i<reducedTokens.size(); ++i){
        ti = reducedTokens.at(i);
        if(ti->tokenType == TokenInfo::Token){
            ParseTreeNode *childNode = new ParseTreeNode();
            childNode->tokenInfo = ti;
            newNode->children.prepend(childNode);
        }else{
            newNode->children.prepend(ruleNodesStack.pop());
        }
    }

    ruleNodesStack.push(newNode);
}

void PlSqlTreeBuilder::accepted()
{
    Q_ASSERT(rootNode==0);

    rootNode = new ParseTreeNode();
    while(!ruleNodesStack.isEmpty()){
        rootNode->children.prepend(ruleNodesStack.pop());
    }
}
