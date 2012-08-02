#ifndef DBOBJECTCOMPAREHELPER_H
#define DBOBJECTCOMPAREHELPER_H

#include <QObject>
#include "connectivity/ociexception.h"
#include "dbobjectcomparehelper.h"
#include "navtree/dbtreemodel.h"
#include "beans/dbobjectinfo.h"

class DbObjectsCompareHelper;
class MetadataLoader;
class SchemaComparisonOptions;

class DbObjectCompareHelper : public QObject
{
    Q_OBJECT
public:
    explicit DbObjectCompareHelper(DbTreeModel::DbTreeNodeType objectType, const QString &compareResult, const QString &currentObjectName, QObject *parent);
    virtual ~DbObjectCompareHelper();
    
    void generateDdl();

signals:
    void changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult);
    void compareInfoReady(const QString &startDdl, const QString &endDdl, DbObjectCompareHelper *dbObjectComparer);
    void compareError(const QString &taskName, const OciException &ex, DbObjectCompareHelper *dbObjectComparer);

protected slots:
    void sourceObjectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void targetObjectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);
    
protected:
    virtual QPair<QString,QString> generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options)=0;
    virtual QPair<QString,QString> generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options)=0;


    QString compareResult;
    QString currentObjectName;
    DbObjectsCompareHelper *dbObjectsCompareHelper;

    bool needsTargetObject;
private:
    DbTreeModel::DbTreeNodeType objectType;
    DbObjectInfo *sourceObjectInfo;
    
};

#endif // DBOBJECTCOMPAREHELPER_H
