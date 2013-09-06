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
    Q_UNUSED(parsingTable);

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

        if(i==0){
            ParseTreeNode *lastChild = newNode->children.last();
            newNode->tokenInfo->endPos = lastChild->tokenInfo->endPos;
            newNode->tokenInfo->endLine = lastChild->tokenInfo->endLine;
            newNode->tokenInfo->endLinePos = lastChild->tokenInfo->endLinePos;
        }else if(i==reducedTokens.size()-1){
            ParseTreeNode *firstChild = newNode->children.first();
            newNode->tokenInfo->startPos = firstChild->tokenInfo->startPos;
            newNode->tokenInfo->startLine = firstChild->tokenInfo->startLine;
            newNode->tokenInfo->startLinePos = firstChild->tokenInfo->startLinePos;
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

void PlSqlTreeBuilder::error()
{
    accepted();
}

ParseTreeNode *PlSqlTreeBuilder::getNode(const QList<int> rulesPath) const
{
    if(!rootNode){
        return 0;
    }

    ParseTreeNode *n = rootNode;
    for(int i=0; i<rulesPath.size(); ++i){
        n = findNode(n, rulesPath.at(i));
        if(!n){
            return 0;
        }
    }

    return n;
}

ParseTreeNode *PlSqlTreeBuilder::findNode(ParseTreeNode *parentNode, int ruleId) const
{
    ParseTreeNode *n;
    for(int i=0; i<parentNode->children.size(); ++i){
        n = parentNode->children.at(i);
        if(n->tokenInfo->tokenType == TokenInfo::Rule &&
                n->tokenInfo->tokenOrRuleId == ruleId){
            return n;
        }
    }

    return 0;
}
