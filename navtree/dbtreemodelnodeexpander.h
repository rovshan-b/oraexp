#ifndef DBTREEMODELNODEEXPANDER_H
#define DBTREEMODELNODEEXPANDER_H

#include <QObject>
#include <QModelIndex>
#include <QQueue>
#include "dbtreemodel.h"

class DbTreeModelNodeExpander : public QObject
{
    Q_OBJECT
public:
    explicit DbTreeModelNodeExpander(DbTreeModel *treeModel);

    void loadChildren(const QModelIndex &parent,
                      QList<DbTreeModel::DbTreeNodeType> nodeTypes=QList<DbTreeModel::DbTreeNodeType>());
    
signals:
    void loadComplete();
    void loadError(const OciException &exception);

private slots:
    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);
    
private:
    int currentIx;
    QModelIndex parentIndex;
    QList<DbTreeModel::DbTreeNodeType> nodeTypesToLoad;

    void load();
    
};

#endif // DBTREEMODELNODEEXPANDER_H
