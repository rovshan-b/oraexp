#ifndef CODESTRUCTUREMODEL_H
#define CODESTRUCTUREMODEL_H

#include <QAbstractItemModel>
#include <QSet>

class PlSqlTreeBuilder;
class ParseTreeNode;
class CodeStructureTreeItem;
class CodeStructureTreeView;

class CodeStructureModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CodeStructureModel(ParseTreeNode *rootNode, QObject *parent);
    virtual ~CodeStructureModel();
    
    QVariant data(const QModelIndex &index, int role) const;

    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const;

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore (const QModelIndex & parent);

    QModelIndex setCursorPosition(int position);
    CodeStructureTreeItem *findItemForPosition(CodeStructureTreeItem *parentItem, int position) const;

private:
    CodeStructureTreeItem *rootItem;

    void populateChildNodes(const QModelIndex & parent);

    bool isValidIndex(const QModelIndex &index) const;

    int cursorPosition;

    QSet<CodeStructureTreeItem*> highlightedItems;

#ifdef DEBUG
    static int instanceCount;
#endif
    
};

#endif // CODESTRUCTUREMODEL_H
