#ifndef TABLEINFOLOADER_H
#define TABLEINFOLOADER_H

#include <QObject>
#include <QQueue>
#include "connectivity/ociexception.h"
#include "connectivity/queryresult.h"
#include "beans/tableinfo.h"
#include "util/param.h"
#include "../metadataloader.h"

#include "tablegeneralinfoloader.h"
#include "tablecolumninfoloader.h"
#include "tableconstraintsloader.h"
#include "tableindexesloader.h"
#include "tableexternalinfoloader.h"
#include "partitioninfoloader.h"
#include "tabletriggersloader.h"
#include "../object_grant/objectgrantsloader.h"

class TableInfoLoader : public MetadataLoader
{
    Q_OBJECT
public:
    enum TableInfoPart
    {
        TableInfoPartGeneralInfo,
        TableInfoPartColumnInfo,
        TableInfoPartConstraintInfo,
        TableInfoPartCheckConstraintInfo,
        TableInfoPartIndexInfo,
        TableInfoPartPartitionInfo,
        TableInfoPartExternalInfo,
        TableInfoPartTriggerInfo,
        TableInfoPartGrantInfo
    };

    explicit TableInfoLoader(IQueryScheduler *queryScheduler,
                             const QString &schemaName,
                             const QString &tableName,
                             QObject *parent,
                             bool loadTriggerInfo=false);
    ~TableInfoLoader();

    virtual void loadObjectInfo();

    QList<Param*> getBasicQueryParams() const;

private slots:
    void tableGeneralInfoReady(const TableGeneralInfo &tableGeneralInfo);
    void tableColumnInfoReady(const QList<ColumnInfo> &tableColumns);
    void tableConstraintInfoReady(const QList<ConstraintInfo> &tableConstraints, bool checkConstraints);
    void tableIndexInfoReady(const QList<IndexInfo> &tableIndexes);
    void partitionInfoReady(const TablePartitioningInfo &pInfo, PartitionInfoLoader *loader);
    void paritionInfoLoadError(const QString &taskName, const OciException &ex, PartitionInfoLoader *loader);
    void tableExternalInfoReady(const TableExternalInfo &tableExternalInfo);
    void tableTriggerInfoReady(const QList<TriggerInfo> &tableTriggers);
    void tableGrantInfoReady(const QList<ObjectGrantInfo> &tableGrants);

    void emitReadySignal();

    void subInfoLoadError(const QString &taskName, const OciException &ex);

private:
    void loadTablePartInfo();

    TableInfo *tableInfo;


    bool loadTriggerInfo;

    TableGeneralInfoLoader generalInfoLoader;
    TableColumnInfoLoader columnListLoader;
    TableConstraintsLoader constraintListLoader;
    TableIndexesLoader indexListLoader;
    PartitionInfoLoader partitionInfoLoader;
    TableExternalInfoLoader externalInfoLoader;
    TableTriggersLoader triggerListLoader;
    ObjectGrantsLoader grantListLoader;

    QQueue<TableInfoPart> partsToLoad;
};

#endif // TABLEINFOLOADER_H
