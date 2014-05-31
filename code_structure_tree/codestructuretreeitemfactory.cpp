#include "codestructuretreeitemfactory.h"
#include "codestructuredeclarationtreeitem.h"
#include "codestructurestatementtreeitem.h"
#include "codestructureparametertreeitem.h"
#include "codestructuregenerictreeitem.h"

CodeStructureTreeItemFactory::CodeStructureTreeItemFactory()
{
}

CodeStructureTreeItem *CodeStructureTreeItemFactory::createByName(const QString &className, ParseTreeNode *node)
{
    if(className == "CodeStructureDeclarationTreeItem"){
        return new CodeStructureDeclarationTreeItem(node);
    }else if(className == "CodeStructureStatementTreeItem"){
        return new CodeStructureStatementTreeItem(node);
    }else if(className == "CodeStructureParameterTreeItem"){
        return new CodeStructureParameterTreeItem(node);
    }else if(className == "CodeStructureGenericTreeItem"){
        return new CodeStructureGenericTreeItem(node);
    }else{
        return 0;
    }
}
