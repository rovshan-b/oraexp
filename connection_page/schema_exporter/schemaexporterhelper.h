#ifndef SCHEMAEXPORTERHELPER_H
#define SCHEMAEXPORTERHELPER_H

#include "connection_page/schema_operation/schemaoperationhelper.h"

class SchemaExportOptions;
class SchemaExporterThread;

class SchemaExporterHelper : public SchemaOperationHelper
{
    Q_OBJECT
public:
    explicit SchemaExporterHelper(const QString &schemaName, IQueryScheduler *queryScheduler,
                                  DbTreeModel *model, SchemaExportOptions *options,
                                  QObject *parent);

    virtual ~SchemaExporterHelper();

    virtual void stop();

protected:
    virtual void startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames);

private slots:
    void objectExported(DbTreeModel::DbTreeNodeType parentNodeType,
                        const QString &objectName);

private:
    SchemaExportOptions *options;

    SchemaExporterThread *workerThread;
    
};

#endif // SCHEMAEXPORTERHELPER_H
