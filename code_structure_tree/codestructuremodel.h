#ifndef CODESTRUCTUREMODEL_H
#define CODESTRUCTUREMODEL_H

#include <QAbstractItemModel>

class PlSqlTreeBuilder;
class CodeStructureTreeItem;

class CodeStructureModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CodeStructureModel(PlSqlTreeBuilder *treeBuilder, QObject *parent);
    virtual ~CodeStructureModel();
    
    QVariant data(const QModelIndex &index, int role) const;

    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const;

private:
    PlSqlTreeBuilder *treeBuilder;

    CodeStructureTreeItem *rootItem;

    bool isValidIndex(const QModelIndex &index) const;
    
};

#endif // CODESTRUCTUREMODEL_H
