#ifndef SCHEMAEXPORTERWORKER_H
#define SCHEMAEXPORTERWORKER_H

#include <QStringList>
#include <QIODevice>

#include "util/stopablethread.h"
#include "navtree/dbtreemodel.h"

class SchemaExportOptions;
class DbObjectInfo;
class MetadataLoader;
class QTextStream;
class QFile;

class SchemaExporterWorker : public QObject
{
    Q_OBJECT
public:
    explicit SchemaExporterWorker(DbTreeModel::DbTreeNodeType parentNodeType,
                                  const QString &schemaName,
                                  const QStringList &objectNames,
                                  SchemaExportOptions *options,
                                  QIODevice::OpenMode fileOpenMode,
                                  IQueryScheduler *queryScheduler,
                                  QObject *parent);

    virtual ~SchemaExporterWorker();

    void start();
    void stop();

signals:
    void statusChanged(const QString &newStatus);
    void exportCompleted();
    void exportError(const QString &taskName, const OciException &ex);
    void objectExportStarted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);
    void objectExported(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);

private slots:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    DbTreeModel::DbTreeNodeType parentNodeType;
    QString schemaName;
    QStringList objectNames;
    SchemaExportOptions *options;
    QIODevice::OpenMode fileOpenMode;
    IQueryScheduler *queryScheduler;

    bool stopped;

    QTextStream *textStream;
    QFile *file;

    QString currentObjectName;

    QString endDdl;

    void loadNextObject();

    void cleanup();
    
};

#endif // SCHEMAEXPORTERWORKER_H
