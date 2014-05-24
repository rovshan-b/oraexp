#include "codestructureparameterlisttreeitem.h"

CodeStructureParameterListTreeItem::CodeStructureParameterListTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    setItemText(QObject::tr("Parameters"));
    setIconName("parameters");
}
