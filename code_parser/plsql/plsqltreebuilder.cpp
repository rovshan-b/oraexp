#include "plsqltreebuilder.h"
#include "beans/tokeninfo.h"
#include "beans/parsetreenode.h"
#include "plsqlrules.h"

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
    }

    setStartEndPositions(newNode);

    ruleNodesStack.push(newNode);
}

void PlSqlTreeBuilder::setStartEndPositions(ParseTreeNode *parentNode)
{
    ParseTreeNode *firstChild=0, *lastChild=0;
    firstLastNonNullChildren(parentNode, &firstChild, &lastChild);

    if(firstChild){
        parentNode->tokenInfo->startPos = firstChild->tokenInfo->startPos;
        parentNode->tokenInfo->startLine = firstChild->tokenInfo->startLine;
        parentNode->tokenInfo->startLinePos = firstChild->tokenInfo->startLinePos;
    }

    if(lastChild){
        parentNode->tokenInfo->endPos = lastChild->tokenInfo->endPos;
        parentNode->tokenInfo->endLine = lastChild->tokenInfo->endLine;
        parentNode->tokenInfo->endLinePos = lastChild->tokenInfo->endLinePos;
    }
}

void PlSqlTreeBuilder::firstLastNonNullChildren(ParseTreeNode *parentNode, ParseTreeNode **first, ParseTreeNode **last) const
{
    for(int i=0; i<parentNode->children.size(); ++i){
        ParseTreeNode *child = parentNode->children.at(i);
        if(child->tokenInfo->startPos!=-1){
            if(!(*first)){
                *first = child;
            }
            *last = child;
        }
    }
}

void PlSqlTreeBuilder::accepted()
{
    Q_ASSERT(rootNode==0);

    rootNode = new ParseTreeNode();
    rootNode->tokenInfo = new TokenInfo();
    rootNode->tokenInfo->tokenType = TokenInfo::Rule;
    rootNode->tokenInfo->tokenOrRuleId = R_START_RULE_AUG;
    while(!ruleNodesStack.isEmpty()){
        rootNode->children.prepend(ruleNodesStack.pop());
    }
    setStartEndPositions(rootNode);
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
