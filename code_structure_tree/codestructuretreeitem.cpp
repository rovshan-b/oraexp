#include "codestructuretreeitem.h"
#include "codestructuretreeitemfactory.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "util/iconutil.h"

CodeStructureTreeItem::CodeStructureTreeItem(ParseTreeNode *node) :
    node(node),
    containsChildren(true),
    childrenPopulated(false)
{
}

CodeStructureTreeItem::~CodeStructureTreeItem()
{
    qDeleteAll(children);
}

void CodeStructureTreeItem::appendChild(CodeStructureTreeItem *child)
{
    child->parentItem = this;
    children.append(child);
}

QVariant CodeStructureTreeItem::data(int role) const
{
    switch(role){
    case Qt::DisplayRole:
        return itemText;
        break;
    case Qt::DecorationRole:
        return IconUtil::getIcon(getIconName());
        break;
    default:
        return QVariant();
        break;
    }
}

int CodeStructureTreeItem::row() const
{
    if(parentItem){
        return parentItem->children.indexOf(const_cast<CodeStructureTreeItem*>(this));
    }

    return 0;
}

bool CodeStructureTreeItem::hasChildren() const
{
    return containsChildren;
}

CodeStructureTreeItem *CodeStructureTreeItem::child(int row)
{
    return children.at(row);
}

int CodeStructureTreeItem::childCount() const
{
    return children.count();
}

CodeStructureTreeItem *CodeStructureTreeItem::parent() const
{
    return parentItem;
}

bool CodeStructureTreeItem::areChildrenPopulated() const
{
    return childrenPopulated;
}

void CodeStructureTreeItem::setChildrenPopulated()
{
    childrenPopulated = true;
}

QList<CodeStructureTreeItem *> CodeStructureTreeItem::populateChildren() const
{
    QList<CodeStructureTreeItem *> results;

    QHash<ParseTreeNode*, QString> nodes = PlSqlTreeBuilder::findNodesWithHandlers(this->node);
    QList<ParseTreeNode*> keys = nodes.keys();
    qSort(keys.begin(), keys.end(), parseTreeNodeLessThan);
    foreach (ParseTreeNode *node, keys) {

        CodeStructureTreeItem *item = CodeStructureTreeItemFactory::createByName(nodes[node], node);
        Q_ASSERT(item);

        results.append(item);
    }

    return results;
}

CodeStructureTreeItem *CodeStructureTreeItem::findChildForPosition(int position) const
{
    return ParseTreeNodeUtil::findChildForPosition(this, position, true);
}

bool CodeStructureTreeItem::containsPosition(int position) const
{
    return node->containsPosition(position);
}

void CodeStructureTreeItem::setItemText(const QString &text)
{
    this->itemText = text;
}

QString CodeStructureTreeItem::getItemText() const
{
    return this->itemText;
}

void CodeStructureTreeItem::setIconName(const QString &name)
{
    this->iconName = name;
}

QString CodeStructureTreeItem::getIconName() const
{
    return this->iconName;
}

void CodeStructureTreeItem::setHasChildren(bool value)
{
    this->containsChildren = value;
}
