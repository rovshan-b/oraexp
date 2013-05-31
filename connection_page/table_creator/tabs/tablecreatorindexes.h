#ifndef TABLECREATORINDEXES_H
#define TABLECREATORINDEXES_H

#include "../tablecreatortabwithtableview.h"
#include "enums.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/indexinfo.h"
#include "models/tableindexesmodel.h"
#include "widgets/datatable.h"
#include "beans/tableinfo.h"

class TableCreatorTabs;
class PartitioningParamsDelegate;
class PartitionInfoLoader;

class TableCreatorIndexes : public TableCreatorTabWithTableView
{
    Q_OBJECT
public:
    explicit TableCreatorIndexes(TableCreatorTabs* objectCreator,
                                 DbObjectCreator::CreatorMode creatorMode,
                                 QWidget *parent);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void currentSchemaChanged(const QString &oldSchemaName, const QString &newSchemaName);

    void setTableInfo(TableInfo *tableInfo);

    QList<IndexInfo> getIndexesInfo() const;
    QList< NameQueryPair > generateAlterDdl() const;

    virtual void removeIncorrectRows();

signals:
    void ddlChanged() const;

private:
    void customizeTableWidget(const QString &schemaName);

    PartitioningParamsDelegate *partititionDefinitionDelegate;

    //these variables and functions are only used in edit mode
    void populateTableWithIndexList();
    void saveStorageParamsToModel(TableIndexesModel *model, int rowIx, const IndexInfo &ixInfo);
    void savePartitionParamsToModel(int rowIx, const TablePartitioningInfo &pInfo, const QString &taskName=QString());

    QList<IndexInfo> *originalIndexList;
    //QList<int> commitedRows;

protected slots:
    virtual void showAdvancedOptions(bool show);

    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);
};

#endif // TABLECREATORINDEXES_H
