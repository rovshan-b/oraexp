#ifndef CODESTRUCTURETREEITEM_H
#define CODESTRUCTURETREEITEM_H

#include <QList>
#include <QVariant>
#include "beans/parsetreenode.h"

class PlSqlTreeBuilder;
class ParseTreeNode;

class CodeStructureTreeItem
{
public:
    CodeStructureTreeItem(ParseTreeNode *node);
    virtual ~CodeStructureTreeItem();

    void appendChild(CodeStructureTreeItem *child);

    QVariant data(int role) const;

    int row() const;
    virtual bool hasChildren() const;
    CodeStructureTreeItem *child(int row);
    int childCount() const;
    CodeStructureTreeItem *parent() const;

    bool areChildrenPopulated() const;
    void setChildrenPopulated();

    virtual QList<CodeStructureTreeItem*> populateChildren() const;

protected:
    ParseTreeNode *node;

    QList<CodeStructureTreeItem*> childItems;
    CodeStructureTreeItem* parentItem;

    void setItemText(const QString &text);
    QString getItemText() const;

    void setIconName(const QString &name);
    QString getIconName() const;

    void setHasChildren(bool value);

private:

    QString itemText;
    QString iconName;
    bool containsChildren;

    bool childrenPopulated;
};

#endif // CODESTRUCTURETREEITEM_H
