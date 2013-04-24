#ifndef SCHEMAOPERATIONHELPER_H
#define SCHEMAOPERATIONHELPER_H

#include <QObject>
#include <QQueue>
#include "navtree/dbtreemodel.h"
#include "interfaces/idbobjectcomparehelper.h"
#include "interfaces/iqueryscheduler.h"

class SchemaOperationHelper : public QObject, public IDbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit SchemaOperationHelper(const QString &schemaName, IQueryScheduler *queryScheduler,
                                   DbTreeModel *model, QObject *parent = 0);
    virtual ~SchemaOperationHelper(){}

    virtual void compare();

signals:
    void statusChanged(const QString &newStatus);
    void completed();
    void comparisonError(const QString &taskName, const OciException &exception);
    void objectCountDetermined(int count);
    void chunkCompleted(int size);
    
protected:
    QString sourceSchema;
    IQueryScheduler *sourceScheduler;
    DbTreeModel *model;

    QQueue<QModelIndex> parentIndexesToCompare;
    QModelIndex currentParentIndex;
    QHash< DbTreeModel::DbTreeNodeType, QQueue<DbTreeModel::DbTreeNodeType> > parentIndexReplacements;
    
    void populateParentIndexesToCompare();
    void compareChildItems();

    void createParentIndexReplacements();
    int getReplacementCount(DbTreeModel::DbTreeNodeType parentNodeType) const;
    DbTreeModel::DbTreeNodeType getNextReplacement(DbTreeModel::DbTreeNodeType parentNodeType);

    void emitStatusChanged(const QString &newStatus);
    void emitCompletedSignal();

    virtual void startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames) = 0;

protected slots:
    void compareNextParentItem();
    void subComparisonError(const QString &taskName, const OciException &exception);

    void objectListLoaded();
    void objectListLoadError(const OciException &exception);
};

#endif // SCHEMAOPERATIONHELPER_H
