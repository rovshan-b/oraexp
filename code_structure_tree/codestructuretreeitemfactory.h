#ifndef CODESTRUCTURETREEITEMFACTORY_H
#define CODESTRUCTURETREEITEMFACTORY_H

#include <QString>

class CodeStructureTreeItem;
class ParseTreeNode;

class CodeStructureTreeItemFactory
{
public:
    static CodeStructureTreeItem *createByName(const QString &className, ParseTreeNode *node);

private:
    CodeStructureTreeItemFactory();
};

#endif // CODESTRUCTURETREEITEMFACTORY_H
