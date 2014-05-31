#include "codestructuregenerictreeitem.h"
#include "code_parser/plsql/plsqlrules.h"
#include "code_parser/plsql/plsqlparsingtable.h"

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
        setItemText(options->guiDisplayName);
    }else{
        QString ruleName = parsingTable->getRuleName(this->node->tokenInfo->tokenOrRuleId);
        ruleName.replace('_', ' ');

        setItemText(ruleName);
    }

    if((options && options->guiNoChildren) || this->node->children.count() == 0){
        setHasChildren(false);
    }
}
