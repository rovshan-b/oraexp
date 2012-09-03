#include "dbobjectscomparehelper.h"
#include "navtree/dbtreeitem.h"
#include "interfaces/iqueryscheduler.h"
#include "defines.h"
#include "dbobjectcomparehelper.h"
#include "dbobjectcomparerfactory.h"
#include "util/dbutil.h"
#include <QDebug>

DbObjectsCompareHelper::DbObjectsCompareHelper(SchemaComparisonOptions *options, QObject *parent) :
    QObject(parent), sourceHasher(this), targetHasher(this), options(options)
{
    connect(&sourceHasher, SIGNAL(hashReady(QString)), this, SLOT(sourceHashReady(QString)));
    connect(&sourceHasher, SIGNAL(hashError(QString,OciException)), this, SIGNAL(comparisonError(QString,OciException)));

    connect(&targetHasher, SIGNAL(hashReady(QString)), this, SLOT(targetHashReady(QString)));
    connect(&targetHasher, SIGNAL(hashError(QString,OciException)), this, SIGNAL(comparisonError(QString,OciException)));
}

DbObjectsCompareHelper::~DbObjectsCompareHelper()
{
    delete options;
    qDebug("deleted DbObjectsCompareHelper");
}

void DbObjectsCompareHelper::compare(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                  const QString &targetSchema, IQueryScheduler *targetScheduler,
                                  const QStringList &objectNames, DbTreeModel::DbTreeNodeType currentObjectsType)
{
    Q_ASSERT(objectNames.size());

    this->sourceSchema=sourceSchema;
    this->sourceScheduler=sourceScheduler;
    this->targetSchema=targetSchema;
    this->targetScheduler=targetScheduler;
    this->objectNames=objectNames;
    this->currentObjectsType=currentObjectsType;
    this->currentObjectsTypeName=DbUtil::getDbObjectTypeNameByNodeType(currentObjectsType);
    this->endDdl.clear();

    currentOffset=0;
    currentChunkSize=0;

    hashNextChunk();
    emit statusChanged(tr("Detecting changed objects..."));
}

void DbObjectsCompareHelper::sourceHashReady(const QString &hash)
{
    qDebug("creating target hash");
    sourceHashingResults.clear();
    sourceHashingResults.append(hash.split(','));
    targetHasher.createHashForObjects(targetScheduler, currentObjectsTypeName, targetSchema, targetSchema, options, objectNames, currentOffset, SCHEMA_COMPARER_CHUNK_SIZE, hash);
}

void DbObjectsCompareHelper::targetHashReady(const QString &hash)
{
    Q_ASSERT(hash.size());
    currentObjectIx=0;
    targetHashingResults.clear();
    targetHashingResults.append(hash.split(','));

    Q_ASSERT(sourceHashingResults.size()==targetHashingResults.size());

    fetchNextObject();
}

void DbObjectsCompareHelper::fetchNextObject()
{
    if(targetHashingResults.isEmpty()){

        currentChunkSize=qMin(objectNames.size()-currentOffset, SCHEMA_COMPARER_CHUNK_SIZE);
        currentOffset+=currentChunkSize;

        if(currentOffset<objectNames.size()){
            hashNextChunk();
        }else{
            emit chunkCompleted(currentChunkSize);
            emit ddlReady(this->endDdl);
            emit comparisonCompleted();
        }

        return;
    }

    QString sourceCompareResult=sourceHashingResults.dequeue();
    QString targetCompareResult=targetHashingResults.dequeue();
    Q_ASSERT(targetCompareResult=="new" ||
             targetCompareResult=="same" ||
             targetCompareResult=="different" ||
             //compareResult=="not_found" || //"not_found" must not exist in target hash
             targetCompareResult=="unsupported_object_type");
    if(targetCompareResult=="same" || /*compareResult=="not_found" ||*/ targetCompareResult=="unsupported_object_type" || sourceCompareResult=="not_found"){ //continue with next one
        currentObjectIx++;
        fetchNextObject();
    }else{
        QString currentObjectName=objectNames.value(currentOffset+currentObjectIx);
        Q_ASSERT(currentObjectName.size());
        currentObjectIx++;

        emit statusChanged(tr("Generating DDL for object %1").arg(currentObjectName));

        DbObjectCompareHelper *compareHelper=DbObjectComparerFactory::createComparerForObjectsType(currentObjectsType,
                                                                                                   targetCompareResult,
                                                                                                   currentObjectName,
                                                                                                   this);
        Q_ASSERT(compareHelper);
        /*if(compareHelper==0){ //unsupported object type
            qDebug() << "Unsupported object type" << currentObjectsType;
            emit comparisonCompleted();
            return;
        }*/

        connect(compareHelper, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), this, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));
        connect(compareHelper, SIGNAL(compareInfoReady(QString,QString,DbObjectCompareHelper*)), this, SLOT(compareInfoReady(QString,QString,DbObjectCompareHelper*)));
        connect(compareHelper, SIGNAL(compareError(QString,OciException,DbObjectCompareHelper*)), this, SLOT(compareError(QString,OciException,DbObjectCompareHelper*)));
        compareHelper->generateDdl();
    }
}

void DbObjectsCompareHelper::compareInfoReady(const QString &startDdl, const QString &endDdl, DbObjectCompareHelper *compareHelper)
{
    qDebug("compareInfoReady");

    if(!startDdl.isEmpty()){
        emit ddlReady(startDdl);
    }

    if(!endDdl.isEmpty()){
        this->endDdl.append(endDdl);
    }

    fetchNextObject();

    compareHelper->deleteLater();
}

void DbObjectsCompareHelper::compareError(const QString &taskName, const OciException &ex, DbObjectCompareHelper *compareHelper)
{
    emit comparisonError(taskName, ex);

    compareHelper->deleteLater();
}

void DbObjectsCompareHelper::hashNextChunk()
{
    if(currentOffset>0){
        Q_ASSERT(currentChunkSize);
        emit chunkCompleted(currentChunkSize);
    }

    emit statusChanged(tr("Detecting changed objects... (%1 out of %2 done)").arg(currentOffset).arg(objectNames.size()));

    qDebug("creating source hash");
    sourceHasher.createHashForObjects(sourceScheduler, currentObjectsTypeName, sourceSchema, targetSchema, options, objectNames, currentOffset, SCHEMA_COMPARER_CHUNK_SIZE);
}
