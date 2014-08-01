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
    static ParseTreeNode *findNode(const ParseTreeNode *parentNode, int ruleId, bool recursive = false);
    static ParseTreeNode *findAnyNode(ParseTreeNode *parentNode, const QList<int> ruleIds, bool recursive = false);
    static ParseTreeNode *findParentNode(ParseTreeNode *childNode, int ruleId, int maxLevels = -1);
    static QHash<ParseTreeNode *, QString> findNodesWithHandlers(ParseTreeNode *parentNode);

    QList<ParseTreeNode *> findDeclarations(int position, ParseTreeNode **discardReason);
    QList<ParseTreeNode *> findDeclarations(const QString &lexeme);

    static void replaceWithProcHeadingNode(ParseTreeNode **procNode);
    static ParseTreeNode *findProcHeadingNode(ParseTreeNode *procNode);

    static bool isProcNode(ParseTreeNode *node);
    static bool areProcNodes(const QList<ParseTreeNode*> &nodes);
    static bool isProcNode(int ruleId);
    static int getPairProcRuleId(int ruleId);
    static ParseTreeNode *getParamListNode(ParseTreeNode *procNode);
    static ParseTreeNode *getParamValuesNode(ParseTreeNode *idNode);
    static int getChildCount(ParseTreeNode *parentNode, int childRuleId);

    static bool isIdentifierOrSeparatorToken(int token);
    static bool isIdentifierToken(int token);
    static bool isIdentifierSeparatorToken(int token);

private:
    ParseTreeNode *rootNode;

    QStack<ParseTreeNode*> ruleNodesStack;

    bool skipEmptyNodes;

    bool calculateCollapsePositions;
    QList<CodeCollapsePosition*> collapsePositions;

    bool calculateScopes;

    void appendChild(ParseTreeNode *parent, ParseTreeNode *child);
    void appendChildren(ParseTreeNode *parent, QList<ParseTreeNode*> children);
    void prependChild(ParseTreeNode *parent, ParseTreeNode *child);

    void setStartEndPositions(ParseTreeNode *parentNode);
    void firstLastNonNullChildren(ParseTreeNode *parentNode, ParseTreeNode **first, ParseTreeNode **last) const;

    void createNewScope(ParseTreeNode *node, ParsingTable *parsingTable);
    void setScopeForNode(ParseTreeNode *node, ParseTreeNodeScope *scope, ParsingTable *parsingTable);
    void registerDeclarationInScope(ParseTreeNodeScope *scope, ParseTreeNode *node, ParsingTable *parsingTable);

    static void fillNodesWithHandlers(QHash<ParseTreeNode*,QString> &nodes, ParseTreeNode *parentNode, ParsingTable *parsingTable);

    static QList<ParseTreeNode *> findTopLevelDeclarationList(ParseTreeNode *parentNode, const QString &lexeme);

};

#endif // PLSQLTREEBUILDER_H
