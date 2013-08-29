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

private:
    ParseTreeNode *rootNode;

    QStack<ParseTreeNode*> ruleNodesStack;

};

#endif // PLSQLTREEBUILDER_H
