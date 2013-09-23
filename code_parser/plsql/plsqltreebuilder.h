#ifndef PLSQLTREEBUILDER_H
#define PLSQLTREEBUILDER_H

#include "../ireducelistener.h"
#include <QStack>

class TokenInfo;
class ParseTreeNode;

class PlSqlTreeBuilder : public IReduceListener
{
public:
    PlSqlTreeBuilder();
    virtual ~PlSqlTreeBuilder();

    ParseTreeNode *getRootNode() const {return this->rootNode;}

    virtual void reduced(TokenInfo* ruleInfo, int symbolCount, const QList<TokenInfo*> &reducedTokens, ParsingTable *parsingTable);
    virtual void accepted();

    virtual void error();

    ParseTreeNode *getNode(const QList<int> rulesPath) const;

private:
    ParseTreeNode *rootNode;

    QStack<ParseTreeNode*> ruleNodesStack;

    ParseTreeNode *findNode(ParseTreeNode *parentNode, int ruleId) const;

    void setStartEndPositions(ParseTreeNode *parentNode);
    void firstLastNonNullChildren(ParseTreeNode *parentNode, ParseTreeNode **first, ParseTreeNode **last) const;

};

#endif // PLSQLTREEBUILDER_H
