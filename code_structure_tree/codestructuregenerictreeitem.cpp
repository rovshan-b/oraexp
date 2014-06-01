#include "codestructuregenerictreeitem.h"
#include "code_parser/plsql/plsqlrules.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "code_parser/plsql/plsqltreebuilder.h"

CodeStructureGenericTreeItem::CodeStructureGenericTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();

    BNFRuleOption *options = parsingTable->ruleOptions.value(this->node->tokenInfo->tokenOrRuleId, 0);
    if(options && !options->guiIconName.isEmpty()){
        setIconName(options->guiIconName);
    }else{
        setIconName("statement");
    }

    if(options && !options->guiDisplayName.isEmpty()){
        QString displayName = options->guiDisplayName;
        if(displayName.compare("$id")==0){
            ParseTreeNode *idNode = PlSqlTreeBuilder::findNode(this->node, R_IDENTIFIER, true);
            if(idNode){
                displayName = idNode->children.at(0)->tokenInfo->lexeme.toLower();
            }else{
                displayName = "";
            }
        }
        setItemText(displayName);
    }

    if(getItemText().isEmpty()){
        QString ruleName = parsingTable->getRuleName(this->node->tokenInfo->tokenOrRuleId);
        ruleName.replace('_', ' ');

        setItemText(ruleName);
    }

    if((options && options->guiNoChildren) || this->node->children.count() == 0){
        setHasChildren(false);
    }
}
