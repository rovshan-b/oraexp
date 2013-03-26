#ifndef DBOBJECTSCOMPAREHELPER_H
#define DBOBJECTSCOMPAREHELPER_H

#include <QObject>
#include <QStringList>
#include <QQueue>
#include "connectivity/ociexception.h"
#include "../dbobjecthasher.h"
#include "navtree/dbtreemodel.h"
#include "beans/schemacomparisonoptions.h"

class IQueryScheduler;
class DbTreeItem;
class DbObjectCompareHelper;

class DbObjectsCompareHelper : public QObject
{
    Q_OBJECT
public:
    explicit DbObjectsCompareHelper(SchemaComparisonOptions *options, QObject *parent);
    ~DbObjectsCompareHelper();

    void compare(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                 const QString &targetSchema, IQueryScheduler *targetScheduler,
                 const QStringList &objectNames, DbTreeModel::DbTreeNodeType currentObjectsType);

    QString getSourceSchemaName() const {return this->sourceSchema;}
    QString getTargetSchemaName() const {return this->targetSchema;}
    IQueryScheduler *getSourceQueryScheduler() const {return this->sourceScheduler;}
    IQueryScheduler *getTargetQueryScheduler() const {return this->targetScheduler;}

    const SchemaComparisonOptions *compareOptions() const {return this->options;}

    void stop() {this->stopped=true;}
    void resetState() {this->stopped=false;}

signals:
    void changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult);
    void statusChanged(const QString &newStatus);
    void ddlReady(const QString &ddl);
    void comparisonCompleted();
    void chunkCompleted(int size);
    void comparisonError(const QString &taskName, const OciException &exception);

private slots:
    void sourceHashReady(const QString &hash);
    void targetHashReady(const QString &hash);
    void fetchNextObject();

    void compareInfoReady(const QString &startDdl, const QString &endDdl, DbObjectCompareHelper *compareHelper);
    void compareError(const QString &taskName, const OciException &ex, DbObjectCompareHelper *compareHelper);
private:
    int currentOffset;
    DbObjectHasher sourceHasher;
    DbObjectHasher targetHasher;
    SchemaComparisonOptions *options;

    QString sourceSchema;
    IQueryScheduler *sourceScheduler;
    QString targetSchema;
    IQueryScheduler *targetScheduler;
    QStringList objectNames;
    DbTreeModel::DbTreeNodeType currentObjectsType;
    QString currentObjectsTypeName;

    QQueue<QString> sourceHashingResults;
    QQueue<QString> targetHashingResults;
    int currentObjectIx;

    QString endDdl;

    void hashNextChunk();

    int currentChunkSize;

    bool stopped;

};

#endif // DBOBJECTSCOMPAREHELPER_H
