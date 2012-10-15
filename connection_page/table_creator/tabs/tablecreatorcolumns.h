#ifndef TABLECREATORCOLUMNS_H
#define TABLECREATORCOLUMNS_H

#include "../tablecreatortabwithtableview.h"
#include "interfaces/istringlistretriever.h"
#include "enums.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"
#include "beans/columninfo.h"
#include "util/param.h"
#include "models/tablecolumnsmodel.h"
#include "widgets/datatable.h"
#include "beans/tableinfo.h"
#include <QModelIndex>

class TableCreatorColumns : public TableCreatorTabWithTableView,
                            public IStringListRetriever
{
    Q_OBJECT
public:
    explicit TableCreatorColumns(TableCreatorTabs *objectCreator, bool editMode, QWidget *parent);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QStringList getStringList(const QString &type=QString());

    void setTableType(OraExp::TableType tableType);
    void setTableInfo(TableInfo *tableInfo);

    QList<ColumnInfo> getColumnsInfo() const;

    QString generateCommentsDdl(const QString &schema, const QString &tableName) const;

signals:
    void ddlChanged() const;

private:
    void customizeTableWidget();
    ColumnInfo columnInfoFromModelRow(const TableColumnsModel *model, int rowIx) const;

    //these variables and functions are only used in edit mode
    void populateTableWithColumnList();
    void saveLobParamsToModel(TableColumnsModel *model, int rowIx, const LobParams &lobParams);
    void disableColumnsForFrozenRows(int rowIx);

    QList<ColumnInfo> *originalColumnList;

protected slots:
    virtual void showAdvancedOptions(bool show);

    //void beforeAlterTable(){static_cast<TableColumnsModel*>(table->model())->removeIncorrectRows();}
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &/*taskName*/){}

    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);
};

#endif // TABLECREATORCOLUMNS_H
