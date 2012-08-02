#ifndef SCHEMACOMPARERHELPER_H
#define SCHEMACOMPARERHELPER_H

#include <QObject>
#include <QQueue>
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreeitem.h"
#include "dbobjectscomparehelper.h"
#include "beans/schemacomparisonoptions.h"
#include "interfaces/idbobjectcomparehelper.h"

//class TablesCompareHelper;

class SchemaComparerHelper : public QObject, public IDbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit SchemaComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                  const QString &targetSchema, IQueryScheduler *targetScheduler,
                                  DbTreeModel *model, SchemaComparisonOptions *options, QObject *parent);

    ~SchemaComparerHelper();

    void compare();
signals:
    void changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult);
    void statusChanged(const QString &newStatus);
    void ddlReady(const QString &ddl);
    void completed();
    void comparisonError(const QString &taskName, const OciException &exception);
    void objectCountDetermined(int count);
    void chunkCompleted(int size);

private slots:
    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);

    void objectComparisonCompleted();
    void subComparisonError(const QString &taskName, const OciException &exception);

private:
    QString sourceSchema;
    QString targetSchema;
    IQueryScheduler *sourceScheduler;
    IQueryScheduler *targetScheduler;
    DbTreeModel *model;
    int totalItemCountToCompare;

    QQueue<QModelIndex> parentIndexesToCompare;
    QModelIndex currentParentIndex;
    QHash< DbTreeModel::DbTreeNodeType, QQueue<DbTreeModel::DbTreeNodeType> > parentIndexReplacements;

    DbObjectsCompareHelper dbObjectsComparer;

    void populateParentIndexesToCompare();
    void loadNextItemList();
    void compareNextItem();
    void loadItemList();
    void compareItems();
    QStringList getCheckedChildNames(const QModelIndex &parent);
    int getCheckedChildCount(const QModelIndex &parent);

    void createParentIndexReplacements();
    int getReplacementCount(DbTreeModel::DbTreeNodeType parentNodeType) const;
    DbTreeModel::DbTreeNodeType getNextReplacement(DbTreeModel::DbTreeNodeType parentNodeType);

    void emitCompletedSignal();

};

#endif // SCHEMACOMPARERHELPER_H
