#ifndef TABLECREATORPARTITIONS_H
#define TABLECREATORPARTITIONS_H

#include "../tablecreatortab.h"
#include "enums.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/tablepartitioninginfo.h"
#include "metadata_loaders/table/partitioninfoloader.h"
#include "beans/tableinfo.h"

class DbConnection;
class QTabWidget;
class TableCreatorPartitionsTab;
class TableCreatorSubpartitionTemplateTab;
class TableCreatorTabs;

class TableCreatorPartitions : public TableCreatorTab
{
    Q_OBJECT
public:
    explicit TableCreatorPartitions(TableCreatorTabs* objectCreator, DbObjectCreator::CreatorMode creatorMode, bool configureForIndex=false, QWidget *parent = 0);
    virtual ~TableCreatorPartitions();

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setTableInfo(TableInfo *tableInfo);

    QStringList tablespaceList;

    TablePartitioningInfo getPartitioningInfo(bool includeIncorrectRows) const;
    void setPartitioningInfo(const TablePartitioningInfo &pInfo, const TablePartitioningInfo &originalPInfo);

    //QString generateDdl() const;
    QList< NameQueryPair > generateAlterDdl() const;
    QList< NameQueryPair > generateAlterDdl(const QString &indexSchema, const QString &indexName) const;

    void removeIncorrectRows();
    bool beforeAlterTable();

    PartitioningInfo *getPartitionPartInfo(OraExp::PartitionPart partitionPart);

signals:
    void ddlChanged() const;
    void currentTabActivated();

private slots:
    void partitionTypeChanged(int);
    void tabIndexChanged(int tabIx);
    void tableTypeChanged(OraExp::TableType tableType);

    void tablespaceListQueryCompleted(const QueryResult &result);
    void tablespaceRecordFetched(const FetchResult &result);
    void tablespaceListFetchCompleted(const QString &/*taskName*/);

    //slots used in edit mode 
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &/*taskName*/){}

private:
    QTabWidget *tab;
    TableCreatorPartitionsTab *partitionsTab;
    TableCreatorPartitionsTab *subpartitionsTab;
    TableCreatorPartitionsTab *subpartitionTemplateTab;

    bool configureForIndex;

    bool canHaveSubpartitions(OraExp::PartitionType partitionType) const;
    bool canHaveSubpartitionTemplate(OraExp::PartitionType spType) const;

    void addSubpartitionsTab();
    void removeSubpartitionsTab();

    void addSubpartitionTemplateTab();
    void removeSubpartitionTemplateTab();

    //functions and variables for edit mode
    void setChildrenCreatorMode(DbObjectCreator::CreatorMode creatorMode);
    TablePartitioningInfo *originalPartitioningInfo;
    bool originalPartitioningInfoIsOnHeap;
};

#endif // TABLECREATORPARTITIONS_H
