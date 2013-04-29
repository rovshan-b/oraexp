#ifndef SCHEMAEXPORTERHELPER_H
#define SCHEMAEXPORTERHELPER_H

#include "connection_page/schema_operation/schemaoperationhelper.h"

class SchemaExportOptions;
class SchemaExporterWorker;
class QTextStream;
class QFile;

class SchemaExporterHelper : public SchemaOperationHelper
{
    Q_OBJECT
public:
    explicit SchemaExporterHelper(const QString &schemaName, IQueryScheduler *queryScheduler,
                                  DbTreeModel *model, SchemaExportOptions *options,
                                  QObject *parent);

    virtual ~SchemaExporterHelper();

    virtual void stop();

signals:
    void objectExportStarted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);
    void objectExportCompleted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);

protected:
    virtual void startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames);

private slots:
    void objectExported(DbTreeModel::DbTreeNodeType parentNodeType,
                        const QString &objectName);
    void exportCompleted();

private:
    SchemaExportOptions *options;

    SchemaExporterWorker *worker;

    bool justStarted;

    void deleteWorker();
    
};

#endif // SCHEMAEXPORTERHELPER_H
