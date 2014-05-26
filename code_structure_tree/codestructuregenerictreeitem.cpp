#include "codestructuregenerictreeitem.h"
#include "code_parser/plsql/plsqlrules.h"
#include "code_parser/plsql/plsqlparsingtable.h"

CodeStructureGenericTreeItem::CodeStructureGenericTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();
    QString ruleName = parsingTable->getRuleName(this->node->tokenInfo->tokenOrRuleId);
    ruleName.replace('_', ' ');

    setItemText(ruleName);

    BNFRuleOption *options = parsingTable->ruleOptions.value(this->node->tokenInfo->tokenOrRuleId, 0);
    if(options && !options->guiIconName.isEmpty()){
        setIconName(options->guiIconName);
    }else{
        setIconName("statement");
    }

    if(options && options->guiNoChildren){
        setHasChildren(false);
    }
}
