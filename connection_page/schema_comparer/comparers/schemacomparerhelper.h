#ifndef SCHEMACOMPARERHELPER_H
#define SCHEMACOMPARERHELPER_H

#include "connection_page/schema_operation/schemaoperationhelper.h"
#include "dbobjectscomparehelper.h"

//class TablesCompareHelper;

class SchemaComparerHelper : public SchemaOperationHelper
{
    Q_OBJECT
public:
    explicit SchemaComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                  const QString &targetSchema, IQueryScheduler *targetScheduler,
                                  DbTreeModel *model, SchemaComparisonOptions *options, QObject *parent);

    ~SchemaComparerHelper();

    void compare();
    virtual void stop();
signals:
    void changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult);
    void ddlReady(const QString &ddl);

protected:
    void startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames);

private:
    QString targetSchema;
    IQueryScheduler *targetScheduler;

    DbObjectsCompareHelper dbObjectsComparer;

};

#endif // SCHEMACOMPARERHELPER_H
