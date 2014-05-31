#include "plsqltreebuilder.h"
#include "beans/tokeninfo.h"
#include "beans/parsetreenode.h"
#include "beans/codecollapseposition.h"
#include "beans/parsetreenodescope.h"
#include "code_parser/parsingtable.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "plsqlrules.h"
#include <QHash>

PlSqlTreeBuilder::PlSqlTreeBuilder() :
    rootNode(0),
    calculateCollapsePositions(false),
    calculateScopes(false)
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
            newNode->children.prepend(childNode);
        }else{
            ParseTreeNode *childNode = ruleNodesStack.pop();
            //check child node options
            if(childNode->children.size() == 0){
                delete childNode;
                continue;
            }

            //process rule options
            BNFRuleOption *options = parsingTable->ruleOptions.value(childNode->tokenInfo->tokenOrRuleId, 0);
            if(options && (options->skip || options->noChildren || (options->scope && calculateScopes))){

                if(options->skip){ //skip this node and add its children
                    newNode->children.append(childNode->children);
                    qSort(newNode->children.begin(), newNode->children.end(), parseTreeNodeLessThan);

                    //detach children and delete
                    childNode->children.clear();
                    delete childNode;
                }else if(options->noChildren){ //do not add children to tree

                    newNode->children.prepend(childNode);

                    qDeleteAll(childNode->children);
                    childNode->children.clear();
                }else if(options->scope){ //find all declarations and set this node as scope for all child nodes not already having scope

                    newNode->children.prepend(childNode);

                    createNewScope(childNode, parsingTable);
                }

            }else{
                newNode->children.prepend(childNode);
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
            scope->declarations[idNode->children[0]->tokenInfo->lexeme] = node;
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
        rootNode->children.prepend(ruleNodesStack.pop());
    }
    setStartEndPositions(rootNode);

    if(calculateScopes){
        createNewScope(rootNode, parsingTable);
    }
}

void PlSqlTreeBuilder::error(ParsingTable *parsingTable)
{
    accepted(parsingTable);
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
