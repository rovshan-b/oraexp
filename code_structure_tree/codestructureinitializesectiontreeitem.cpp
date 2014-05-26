#include "codestructureinitializesectiontreeitem.h"

CodeStructureInitializeSectionTreeItem::CodeStructureInitializeSectionTreeItem(ParseTreeNode *node) : CodeStructureTreeItem(node)
{
    setItemText(QObject::tr("Initialization"));
    setIconName("folder");
}
