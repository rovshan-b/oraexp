#include "codestructuretreeitemfactory.h"
#include "codestructuredeclarationtreeitem.h"
#include "codestructureparameterlisttreeitem.h"
#include "codestructureparametertreeitem.h"

CodeStructureTreeItemFactory::CodeStructureTreeItemFactory()
{
}

CodeStructureTreeItem *CodeStructureTreeItemFactory::createByName(const QString &className, ParseTreeNode *node)
{
    if(className == "CodeStructureDeclarationTreeItem"){
        return new CodeStructureDeclarationTreeItem(node);
    }else if(className == "CodeStructureParameterListTreeItem"){
        return new CodeStructureParameterListTreeItem(node);
    }else if(className == "CodeStructureParameterTreeItem"){
        return new CodeStructureParameterTreeItem(node);
    }else{
        return 0;
    }
}
