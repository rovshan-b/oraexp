#ifndef CODESTRUCTURETREEITEM_H
#define CODESTRUCTURETREEITEM_H

#include <QList>
#include <QVariant>

class ParseTreeNode;

class CodeStructureTreeItem
{
public:
    CodeStructureTreeItem(ParseTreeNode *node);
    ~CodeStructureTreeItem();

    QVariant data(int role) const;

    int row() const;
    bool hasChildren() const;
    CodeStructureTreeItem *child(int row);
    int childCount() const;
    CodeStructureTreeItem *parent() const;

private:
    ParseTreeNode *node;

    QList<CodeStructureTreeItem*> childItems;
    CodeStructureTreeItem* parentItem;
};

#endif // CODESTRUCTURETREEITEM_H
