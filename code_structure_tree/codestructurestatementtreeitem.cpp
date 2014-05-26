#include "codestructurestatementtreeitem.h"
#include "code_parser/plsql/plsqlrules.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "code_parser/plsql/plsqltreebuilder.h"

CodeStructureStatementTreeItem::CodeStructureStatementTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    ParseTreeNode *statementTypeNode = PlSqlTreeBuilder::findNode(node, R_STATEMENT_TYPE, false);

    if(statementTypeNode){
        statementTypeNode = statementTypeNode->children[0];
        Q_ASSERT(statementTypeNode->tokenInfo->tokenType == TokenInfo::Rule);

        if(statementTypeNode->tokenInfo->tokenOrRuleId == R_SQL_STATEMENT){
            statementTypeNode = statementTypeNode->children[0];
        }

        this->node = statementTypeNode;

        PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();
        QString ruleName = parsingTable->getRuleName(this->node->tokenInfo->tokenOrRuleId);
        ruleName.replace("_statement", "").replace('_', ' ');

        setItemText(ruleName);
    }else{ //constructed as a result of error recovery routine
        setItemText("statement");
    }

    setIconName("statement");

    int ruleId = this->node->tokenInfo->tokenOrRuleId;

    setHasChildren(ruleId == R_BASIC_LOOP_STATEMENT ||
                    ruleId == R_CASE_STATEMENT ||
                    ruleId == R_FOR_LOOP_STATEMENT ||
                    ruleId == R_IF_STATEMENT ||
                    ruleId == R_PLSQL_BLOCK ||
                    ruleId == R_WHILE_LOOP_STATEMENT);
}
