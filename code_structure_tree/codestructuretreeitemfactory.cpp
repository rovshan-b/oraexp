#include "codestructuretreeitemfactory.h"
#include "codestructuredeclarationtreeitem.h"
#include "codestructurestatementtreeitem.h"
#include "codestructureparameterlisttreeitem.h"
#include "codestructureparametertreeitem.h"
#include "codestructureinitializesectiontreeitem.h"
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
    }else if(className == "CodeStructureParameterListTreeItem"){
        return new CodeStructureParameterListTreeItem(node);
    }else if(className == "CodeStructureParameterTreeItem"){
        return new CodeStructureParameterTreeItem(node);
    }else if(className == "CodeStructureInitializeSectionTreeItem"){
        return new CodeStructureInitializeSectionTreeItem(node);
    }else if(className == "CodeStructureGenericTreeItem"){
        return new CodeStructureGenericTreeItem(node);
    }else{
        return 0;
    }
}
