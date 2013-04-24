#ifndef SCHEMAEXPORTERTHREAD_H
#define SCHEMAEXPORTERTHREAD_H

#include <QStringList>

#include "util/stopablethread.h"
#include "navtree/dbtreemodel.h"

class SchemaExportOptions;
class DbObjectInfo;
class MetadataLoader;

class SchemaExporterThread : public StopableThread
{
    Q_OBJECT
public:
    explicit SchemaExporterThread(DbTreeModel::DbTreeNodeType parentNodeType,
                                  const QString &schemaName,
                                  const QStringList &objectNames,
                                  SchemaExportOptions *options,
                                  IQueryScheduler *queryScheduler,
                                  QObject *parent);
    
    virtual void run();

signals:
    void statusChanged(const QString &newStatus);
    void exportCompleted();
    void exportError(const QString &taskName, const OciException &ex);
    void objectExported(DbTreeModel::DbTreeNodeType parentNodeType,
                        const QString &objectName);

private slots:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    DbTreeModel::DbTreeNodeType parentNodeType;
    QString schemaName;
    QStringList objectNames;
    SchemaExportOptions *options;
    IQueryScheduler *queryScheduler;

    QString currentObjectName;

    void loadNextObject();
    
};

#endif // SCHEMAEXPORTERTHREAD_H
