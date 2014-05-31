#ifndef CODESTRUCTURETREEITEM_H
#define CODESTRUCTURETREEITEM_H

#include <QList>
#include <QVariant>
#include "beans/parsetreenode.h"

class PlSqlTreeBuilder;
class ParseTreeNode;

class CodeStructureTreeItem
{
    friend bool codeStructureTreeItemLessThan(CodeStructureTreeItem *i1, CodeStructureTreeItem *i2);
    friend bool codeStructureTreeItemNameLessThan(CodeStructureTreeItem *i1, CodeStructureTreeItem *i2);

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

    CodeStructureTreeItem *findChildForPosition(int position) const;

    bool containsPosition(int position) const;

    ParseTreeNode *getNode() const {return this->node;}

    QString getItemText() const;

protected:
    ParseTreeNode *node;

    QList<CodeStructureTreeItem*> childItems;
    CodeStructureTreeItem* parentItem;

    void setItemText(const QString &text);

    void setIconName(const QString &name);
    QString getIconName() const;

    void setHasChildren(bool value);

private:

    QString itemText;
    QString iconName;
    bool containsChildren;

    bool childrenPopulated;
};

inline bool codeStructureTreeItemLessThan(CodeStructureTreeItem *i1, CodeStructureTreeItem *i2)
{
    return i1->node->tokenInfo->startPos < i2->node->tokenInfo->startPos;
}

inline bool codeStructureTreeItemNameLessThan(CodeStructureTreeItem *i1, CodeStructureTreeItem *i2)
{
    return (QString::compare(i1->getItemText(), i2->getItemText(), Qt::CaseInsensitive) < 0);
}

#endif // CODESTRUCTURETREEITEM_H
