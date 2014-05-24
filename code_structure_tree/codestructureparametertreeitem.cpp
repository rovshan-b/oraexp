#include "codestructureparametertreeitem.h"
#include "code_parser/plsql/plsqlrules.h"

CodeStructureParameterTreeItem::CodeStructureParameterTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    Q_ASSERT(node->children.count() == 2);

    ParseTreeNode *idNode = node->children[0];
    setItemText(idNode->children[0]->tokenInfo->lexeme);

    ParseTreeNode *paramTypeNode = node->children[1];
    Q_ASSERT(paramTypeNode->tokenInfo->tokenType == TokenInfo::Rule);

    QString iconName;

    if(paramTypeNode->tokenInfo->tokenOrRuleId == R_IN_PARAM){
        iconName = "right_arrow";
    }else{
        Q_ASSERT(paramTypeNode->tokenInfo->tokenOrRuleId == R_OUT_PARAM);

        ParseTreeNode *inOrOutNode = paramTypeNode->children[0];
        if(inOrOutNode->tokenInfo->tokenType == TokenInfo::Rule){ //opt_in
            iconName = "left_right_arrows";
        }else{ //'OUT'
            iconName = "left_arrow";
        }
    }

    setIconName(iconName);

    setHasChildren(false);
}
