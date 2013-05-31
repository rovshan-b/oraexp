#ifndef TABLESPACEQUOTASEDITORTABLE_H
#define TABLESPACEQUOTASEDITORTABLE_H

#include "datatableandtoolbarwidget.h"
#include "beans/tablespacequotainfo.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class IQueryScheduler;
class IndexBasedComboBoxDelegate;
class TablespaceQuotasModel;

class TablespaceQuotasEditorTable : public DataTableAndToolBarWidget
{
    Q_OBJECT
public:
    explicit TablespaceQuotasEditorTable(DbObjectCreator::CreatorMode creatorMode, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    QList<TablespaceQuotaInfo> getList() const;
    void populateTable(QList<TablespaceQuotaInfo> *originalItemList);

    void removeIncorrectRows();

signals:
    void ddlChanged();

protected slots:
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);
    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

private:
    IQueryScheduler *queryScheduler;

    DbObjectCreator::CreatorMode creatorMode;
    QList<TablespaceQuotaInfo> *originalItemList;

    void customizeTable();
    static void setRowData(int rowIx, TablespaceQuotasModel *model, TablespaceQuotaInfo *info);
    
};

#endif // TABLESPACEQUOTASEDITORTABLE_H
