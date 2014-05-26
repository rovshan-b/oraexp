#include "codestructuredeclarationtreeitem.h"
#include "code_parser/plsql/plsqlrules.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "code_parser/plsql/plsqltreebuilder.h"

CodeStructureDeclarationTreeItem::CodeStructureDeclarationTreeItem(ParseTreeNode *node) :
    CodeStructureTreeItem(node)
{
    PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();

    if(node->children.size()==1){
        ParseTreeNode *childNode = node->children[0];
        if(childNode->tokenInfo->tokenType==TokenInfo::Rule &&
                childNode->tokenInfo->tokenOrRuleId==R_DECLARATION_TYPE){
            Q_ASSERT(childNode->children.size()==1);

            this->node = childNode->children[0];

            BNFRuleOption *options = parsingTable->ruleOptions.value(this->node->tokenInfo->tokenOrRuleId, 0);
            if(options && !options->guiIconName.isEmpty()){
                setIconName(options->guiIconName);
            }

            ParseTreeNode *idNode = PlSqlTreeBuilder::findNode(this->node, R_IDENTIFIER, true);
            if(idNode){
                setItemText(idNode->children.at(0)->tokenInfo->lexeme);
            }
        }
    }

    int ruleId = this->node->tokenInfo->tokenOrRuleId;

    setHasChildren(ruleId == R_PROCEDURE_DECLARATION ||
                    ruleId == R_PROCEDURE_DEFINITION ||
                    ruleId == R_FUNCTION_DECLARATION ||
                    ruleId == R_FUNCTION_DEFINITION);

    if(getItemText().isEmpty()){
        setItemText(parsingTable->getRuleName(this->node->tokenInfo->tokenOrRuleId));
        setIconName("variable");
    }
}
