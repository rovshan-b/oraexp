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

    virtual void error(ParsingTable *parsingTable, QStack<TokenInfo*> &tokenStack);

    void setSkipEmptyNodes(bool skip);
    void setCalculateCollapsePositions();
    QList<CodeCollapsePosition*> getCollapsePositions() const;
    void clearCollapsePositions();

    //ParseTreeNode *getNode(const QList<int> rulesPath) const;
    static ParseTreeNode *findNode(ParseTreeNode *parentNode, int ruleId, bool recursive = false);
    static ParseTreeNode *findAnyNode(ParseTreeNode *parentNode, const QList<int> ruleIds, bool recursive = false);
    static QHash<ParseTreeNode *, QString> findNodesWithHandlers(ParseTreeNode *parentNode);

private:
    ParseTreeNode *rootNode;

    QStack<ParseTreeNode*> ruleNodesStack;

    bool skipEmptyNodes;

    bool calculateCollapsePositions;
    QList<CodeCollapsePosition*> collapsePositions;

    bool calculateScopes;

    void setStartEndPositions(ParseTreeNode *parentNode);
    void firstLastNonNullChildren(ParseTreeNode *parentNode, ParseTreeNode **first, ParseTreeNode **last) const;

    void createNewScope(ParseTreeNode *node, ParsingTable *parsingTable);
    void setScopeForNode(ParseTreeNode *node, ParseTreeNodeScope *scope, ParsingTable *parsingTable);
    void registerDeclarationInScope(ParseTreeNodeScope *scope, ParseTreeNode *node, ParsingTable *parsingTable);

    static void fillNodesWithHandlers(QHash<ParseTreeNode*,QString> &nodes, ParseTreeNode *parentNode, ParsingTable *parsingTable);

};

#endif // PLSQLTREEBUILDER_H
