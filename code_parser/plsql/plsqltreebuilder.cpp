#include "plsqltreebuilder.h"
#include "beans/tokeninfo.h"
#include "beans/parsetreenode.h"
#include "beans/codecollapseposition.h"
#include "beans/parsetreenodescope.h"
#include "code_parser/parsingtable.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "plsqlrules.h"
#include <QHash>

#include <QTime>

PlSqlTreeBuilder::PlSqlTreeBuilder() :
    rootNode(0),
    skipEmptyNodes(true),
    calculateCollapsePositions(false),
    calculateScopes(true)
{
}

PlSqlTreeBuilder::~PlSqlTreeBuilder()
{
    qDeleteAll(ruleNodesStack);
    //delete rootNode->scope;
    delete rootNode;

    qDeleteAll(collapsePositions);
}
/*
void printoutTree(ParseTreeNode *node, const QString &prefix)
{
    qDebug() << QString("%1%2").arg(prefix, node->tokenInfo->toString());
    QString newPrefix = QString("%1   ").arg(prefix);
    for(int i=0; i<node->children.size(); ++i){
        printoutTree(node->children.at(i), newPrefix);
    }
}*/

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
            prependChild(newNode, childNode);
        }else{
            ParseTreeNode *childNode = ruleNodesStack.pop();
            //check child node options
            if(childNode->children.size() == 0 && skipEmptyNodes){
                delete childNode;
                continue;
            }

            //process rule options
            BNFRuleOption *options = parsingTable->ruleOptions.value(childNode->tokenInfo->tokenOrRuleId, 0);
            if(options && (options->skip || options->noChildren || (options->scope && calculateScopes))){

                if(options->skip){ //skip this node and add its children
                    appendChildren(newNode, childNode->children);
                    qSort(newNode->children.begin(), newNode->children.end(), parseTreeNodeLessThan);

                    //detach children and delete
                    childNode->children.clear();
                    delete childNode;
                }else if(options->noChildren){ //do not add children to tree

                    prependChild(newNode, childNode);

                    qDeleteAll(childNode->children);
                    childNode->children.clear();
                }else if(options->scope){ //find all declarations and set this node as scope for all child nodes not already having scope

                    prependChild(newNode, childNode);

                    createNewScope(childNode, parsingTable);
                }

            }else{
                prependChild(newNode, childNode);
            }
            //end process rule options
        }
    }

    setStartEndPositions(newNode);

    ruleNodesStack.push(newNode);

    TokenInfo::TokenType tokenType = newNode->tokenInfo->tokenType;
    int ruleId = newNode->tokenInfo->tokenOrRuleId; //if above type is not Rule this info is wrong (it is token id)
    int startLine = newNode->tokenInfo->startLine;
    int endLine = newNode->tokenInfo->endLine;
    if(calculateCollapsePositions &&
            (endLine-startLine)!=0 &&
            (tokenType==TokenInfo::Rule &&
                ((ruleId==R_BASIC_LOOP_STATEMENT ||
                    ruleId==R_CASE_STATEMENT ||
                    ruleId==R_FOR_LOOP_STATEMENT ||
                    ruleId==R_IF_STATEMENT ||
                    ruleId==R_PLSQL_BLOCK ||
                    ruleId==R_PLSQL_BODY) ||
                    ruleId==R_WHILE_LOOP_STATEMENT ||
                    ruleId==R_CREATE_OBJECT ||
                    ruleId==R_PROCEDURE_DEFINITION ||
                    ruleId==R_FUNCTION_DEFINITION ||
                    ruleId==R_ELSIF_BLOCK ||
                    ruleId==R_OPT_ELSE_BLOCK ||
                    ruleId==R_WHEN_THEN ||
                 ((ruleId==R_STATEMENT || ruleId==R_DECLARATION) && (endLine - startLine)>=3)))){
        int currentCount = collapsePositions.size();

        CodeCollapsePosition *lastPosition = currentCount > 0 ? collapsePositions[currentCount-1] : 0;
        if((lastPosition!=0 && lastPosition->startLine!=startLine) || lastPosition==0){ //do not add two items starting on same position
            collapsePositions.append(new CodeCollapsePosition(startLine, endLine));
        }
    }

    //register declarations here

    //qDebug() << "-----------------------------------";
    //printoutTree(newNode, "");
    //qDebug() << "-----------------------------------";
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

void PlSqlTreeBuilder::createNewScope(ParseTreeNode *node, ParsingTable *parsingTable)
{
    Q_ASSERT(node->scope == 0);

    ParseTreeNodeScope* scope = new ParseTreeNodeScope();
    node->scope = scope;
    node->ownsScope = true;
    setScopeForNode(node, scope, parsingTable);
}

void PlSqlTreeBuilder::setScopeForNode(ParseTreeNode *node, ParseTreeNodeScope *scope, ParsingTable *parsingTable)
{
    foreach(ParseTreeNode *childNode, node->children){
        registerDeclarationInScope(scope, childNode, parsingTable);

        if(childNode->scope != 0){
            return;
        }

        childNode->scope = scope;

        setScopeForNode(childNode, scope, parsingTable);
    }
}

void PlSqlTreeBuilder::registerDeclarationInScope(ParseTreeNodeScope *scope, ParseTreeNode *node, ParsingTable *parsingTable)
{
    if(node->tokenInfo->tokenType == TokenInfo::Rule){
        BNFRuleOption *options = parsingTable->ruleOptions.value(node->tokenInfo->tokenOrRuleId, 0);
        if(options && options->symbolTableEntry){
            ParseTreeNode *idNode = findNode(node, R_IDENTIFIER, true);
            Q_ASSERT(idNode && idNode->children.size()>0);
            scope->declarations.insert(idNode->children[0]->tokenInfo->lexeme.toLower(), node);
        }
    }
}

void PlSqlTreeBuilder::accepted(ParsingTable *parsingTable)
{
    Q_ASSERT(rootNode==0);

    rootNode = new ParseTreeNode();
    rootNode->tokenInfo = new TokenInfo();
    rootNode->tokenInfo->tokenType = TokenInfo::Rule;
    rootNode->tokenInfo->tokenOrRuleId = R_START_RULE_AUG;
    while(!ruleNodesStack.isEmpty()){
        prependChild(rootNode, ruleNodesStack.pop());
    }
    setStartEndPositions(rootNode);

    if(calculateScopes){
        createNewScope(rootNode, parsingTable);
    }
}

void PlSqlTreeBuilder::error(ParsingTable *parsingTable, QStack<TokenInfo *> &tokenStack)
{
    QStack<TokenInfo *> copiedTokenStack(tokenStack);

    foreach(TokenInfo *tokenInfo, copiedTokenStack){
        bool found = false;
        foreach(ParseTreeNode *node, ruleNodesStack){
            if(node->tokenInfo == tokenInfo){
                found = true;
                break;
            }
        }
        if(!found){
            tokenStack.remove(tokenStack.indexOf(tokenInfo));
            delete tokenInfo;
        }
    }

    accepted(parsingTable);
}

void PlSqlTreeBuilder::appendChild(ParseTreeNode *parent, ParseTreeNode *child)
{
    child->parentNode = parent;
    parent->children.append(child);
}

void PlSqlTreeBuilder::appendChildren(ParseTreeNode *parent, QList<ParseTreeNode *> children)
{
    foreach(ParseTreeNode *child, children){
        appendChild(parent, child);
    }
}

void PlSqlTreeBuilder::prependChild(ParseTreeNode *parent, ParseTreeNode *child)
{
    child->parentNode = parent;
    parent->children.prepend(child);
}

void PlSqlTreeBuilder::setSkipEmptyNodes(bool skip)
{
    this->skipEmptyNodes = skip;
}

void PlSqlTreeBuilder::setCalculateCollapsePositions()
{
    this->calculateCollapsePositions = true;
}

QList<CodeCollapsePosition *> PlSqlTreeBuilder::getCollapsePositions() const
{
    return this->collapsePositions;
}

void PlSqlTreeBuilder::clearCollapsePositions()
{
    qDeleteAll(collapsePositions);
    collapsePositions.clear();
}

/*
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
}*/

ParseTreeNode *PlSqlTreeBuilder::findNode(ParseTreeNode *parentNode, int ruleId, bool recursive)
{
    ParseTreeNode *n;
    for(int i=0; i<parentNode->children.size(); ++i){
        n = parentNode->children.at(i);

        if(n->tokenInfo->tokenType != TokenInfo::Rule){
            continue;
        }

        if(n->tokenInfo->tokenOrRuleId == ruleId){
            return n;
        }else if(recursive){
            n = findNode(n, ruleId, recursive);
            if(n != 0){
                return n;
            }
        }
    }

    return 0;
}

ParseTreeNode *PlSqlTreeBuilder::findAnyNode(ParseTreeNode *parentNode, const QList<int> ruleIds, bool recursive)
{
    ParseTreeNode *n;
    for(int i=0; i<parentNode->children.size(); ++i){
        n = parentNode->children.at(i);

        if(n->tokenInfo->tokenType != TokenInfo::Rule){
            continue;
        }

        for(int k=0; k<ruleIds.size(); ++k){
            if(n->tokenInfo->tokenOrRuleId == ruleIds[k]){
                return n;
            }
        }

        if(recursive){
            n = findAnyNode(n, ruleIds, recursive);
            if(n != 0){
                return n;
            }
        }
    }

    return 0;
}

QHash<ParseTreeNode *, QString> PlSqlTreeBuilder::findNodesWithHandlers(ParseTreeNode *parentNode)
{
    QHash<ParseTreeNode *, QString> nodes;

    fillNodesWithHandlers(nodes, parentNode, PlSqlParsingTable::getInstance());

    return nodes;
}

QList<ParseTreeNode*> PlSqlTreeBuilder::findDeclarations(int position, ParseTreeNode **discardReason)
{
    QTime t;
    t.start();
    *discardReason = 0;

    ParseTreeNode *node = rootNode->findChildForPosition(position);

    if(PlSqlParseHelper::isIdentifierToken(node->tokenInfo->tokenOrRuleId)){
        return QList<ParseTreeNode*>();
    }

    if(!node->scope){
        return QList<ParseTreeNode*>();
    }

    QString lexeme = node->tokenInfo->lexeme.toLower(); //scope contains all variable names in lower case

    QList<ParseTreeNode*> declNodeList = node->scope->declarations.values(lexeme);
    while(declNodeList.isEmpty()){
        node = node->parentNode;

        if(node == 0 || node->scope == 0){
            break;
        }

        declNodeList = node->scope->declarations.values(lexeme);
    }

    for(int i=0; i<declNodeList.count(); ++i){
        ParseTreeNode **declNode = &declNodeList[i];
        TokenInfo *tokenInfo = (*declNode)->tokenInfo;

        if(tokenInfo->tokenOrRuleId == R_FUNCTION_DECLARATION ||
                tokenInfo->tokenOrRuleId == R_FUNCTION_DEFINITION ||
                tokenInfo->tokenOrRuleId == R_PROCEDURE_DECLARATION ||
                tokenInfo->tokenOrRuleId == R_PROCEDURE_DEFINITION){

            *declNode = findAnyNode(*declNode, QList<int>() << R_PROCEDURE_HEADING << R_FUNCTION_HEADING, true);
            Q_ASSERT(*declNode);
        }

        if((*declNode)->containsPosition(position)){ //cursor is on declaration itself
            *discardReason = *declNode;
            //return QList<ParseTreeNode*>();
        }
    }

    qDebug() << "found declaration node in" << t.elapsed() << "ms";
    return declNodeList;
}

QList<ParseTreeNode *> PlSqlTreeBuilder::findDeclarations(const QString &lexeme)
{
    return PlSqlTreeBuilder::findTopLevelDeclarationList(rootNode, lexeme.toLower());
}

QList<ParseTreeNode *> PlSqlTreeBuilder::findTopLevelDeclarationList(ParseTreeNode *parentNode, const QString &lexeme) //lexeme must be in lower case
{
    QList<ParseTreeNode*> list = parentNode->scope->declarations.values(lexeme);
    if(!list.isEmpty()){
        return list;
    }

    foreach(ParseTreeNode *childNode, parentNode->children){
        list = PlSqlTreeBuilder::findTopLevelDeclarationList(childNode, lexeme);
        if(!list.isEmpty()){
            return list;
        }
    }

    return list;
}

void PlSqlTreeBuilder::fillNodesWithHandlers(QHash<ParseTreeNode *, QString> &nodes, ParseTreeNode *parentNode, ParsingTable *parsingTable)
{
    ParseTreeNode *n;
    BNFRuleOption *options;
    for(int i=0; i<parentNode->children.size(); ++i){
        n = parentNode->children.at(i);

        if(n->tokenInfo->tokenType != TokenInfo::Rule){
            continue;
        }


        options = parsingTable->ruleOptions.value(n->tokenInfo->tokenOrRuleId, 0);
        if(!options){
            continue;
        }

        if(options->skipInGuiTree){
            fillNodesWithHandlers(nodes, n, parsingTable);
        }else if(!options->guiHandlerName.isEmpty()){
            nodes[n] = options->guiHandlerName;
        }
    }
}
