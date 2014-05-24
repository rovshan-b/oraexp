#ifndef CODESTRUCTUREMODEL_H
#define CODESTRUCTUREMODEL_H

#include <QAbstractItemModel>

class PlSqlTreeBuilder;
class ParseTreeNode;
class CodeStructureTreeItem;

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

private:
    CodeStructureTreeItem *rootItem;

    void populateChildNodes(const QModelIndex & parent);

    bool isValidIndex(const QModelIndex &index) const;

#ifdef DEBUG
    static int instanceCount;
#endif
    
};

#endif // CODESTRUCTUREMODEL_H
