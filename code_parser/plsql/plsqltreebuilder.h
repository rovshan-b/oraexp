#ifndef PLSQLTREEBUILDER_H
#define PLSQLTREEBUILDER_H

#include "../ireducelistener.h"
#include <QStack>

class TokenInfo;
class ParseTreeNode;
class ParseTreeNodeScope;
class CodeCollapsePosition;

class PlSqlTreeBuilder : public IReduceListener
{
public:
    PlSqlTreeBuilder();
    virtual ~PlSqlTreeBuilder();

    ParseTreeNode *getRootNode() const {return this->rootNode;}

    virtual void reduced(TokenInfo* ruleInfo, int symbolCount, const QList<TokenInfo*> &reducedTokens, ParsingTable *parsingTable);
    virtual void accepted(ParsingTable *parsingTable);

    virtual void error(ParsingTable *parsingTable);

    void setCalculateCollapsePositions();
    QList<CodeCollapsePosition*> getCollapsePositions() const;
    void clearCollapsePositions();

    ParseTreeNode *getNode(const QList<int> rulesPath) const;
    ParseTreeNode *findNode(ParseTreeNode *parentNode, int ruleId, bool recursive = false) const;

    ParseTreeNode *findFirstMultiChildNode() const;

private:
    ParseTreeNode *rootNode;

    QStack<ParseTreeNode*> ruleNodesStack;

    bool calculateCollapsePositions;
    QList<CodeCollapsePosition*> collapsePositions;

    bool calculateScopes;

    void setStartEndPositions(ParseTreeNode *parentNode);
    void firstLastNonNullChildren(ParseTreeNode *parentNode, ParseTreeNode **first, ParseTreeNode **last) const;

    void createNewScope(ParseTreeNode *node, ParsingTable *parsingTable);
    void setScopeForNode(ParseTreeNode *node, ParseTreeNodeScope *scope, ParsingTable *parsingTable);
    void registerDeclarationInScope(ParseTreeNodeScope *scope, ParseTreeNode *node, ParsingTable *parsingTable);

};

#endif // PLSQLTREEBUILDER_H
