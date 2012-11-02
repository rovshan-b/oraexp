#include "tablespacequotaseditortable.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include "models/tablespacequotasmodel.h"
#include "delegates/dbitemlistdelegate.h"
#include "delegates/booleandelegate.h"
#include "delegates/numericdelegate.h"
#include "delegates/indexbasedcomboboxdelegate.h"

TablespaceQuotasEditorTable::TablespaceQuotasEditorTable(bool editMode, QWidget *parent) :
    DataTableAndToolBarWidget(0, Qt::Horizontal, parent),
    queryScheduler(0),
    editMode(editMode),
    originalItemList(0)
{
}

void TablespaceQuotasEditorTable::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    customizeTable();
}

QList<TablespaceQuotaInfo> TablespaceQuotasEditorTable::getList() const
{
    return ItemCreatorHelper::getItemList<TablespaceQuotaInfo,TablespaceQuotasModel>(
                static_cast<TablespaceQuotasModel*>(table()->model()));
}

void TablespaceQuotasEditorTable::populateTable(QList<TablespaceQuotaInfo> *originalItemList)
{
    Q_ASSERT(this->originalItemList==0);

    this->originalItemList=originalItemList;
    if(this->originalItemList->size()>0){
        ItemCreatorHelper::populateTableWithItems<TablespaceQuotaInfo,TablespaceQuotasModel>(originalItemList,
                                                                                 this->table(),
                                                                                 &TablespaceQuotasEditorTable::setRowData);
    }
}

void TablespaceQuotasEditorTable::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table());
}

void TablespaceQuotasEditorTable::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalItemList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(table()->model());
    int rowIx=rowNum-1;

    if(!taskName.startsWith("quota_add_")){
        Q_ASSERT(originalItemList->size()>rowIx);
    }

    TablespaceQuotaInfo modifiedQuotaInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("quota_add_tablespace_quota_")){

        originalItemList->append(modifiedQuotaInfo);
        model->freezeRow(rowIx, true);

    }else if(taskName.startsWith("quota_drop_tablespace_quota_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("quota_alter_tablespace_quota_")){

        (*originalItemList)[rowIx]=modifiedQuotaInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalItemList);
}

void TablespaceQuotasEditorTable::alterQueryError(const QString &)
{

}

void TablespaceQuotasEditorTable::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    TablespaceQuotasModel *model=static_cast<TablespaceQuotasModel*>(table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalItemList, from, to);
}

void TablespaceQuotasEditorTable::customizeTable()
{
    DataTable *table=this->table();

    QStringList columnNames;
    columnNames.append(tr("Tablespace"));
    columnNames.append(tr("Unlimited"));
    columnNames.append(tr("Quota"));
    columnNames.append(tr("Unit"));

    TablespaceQuotasModel *tableModel=new TablespaceQuotasModel(columnNames, this);
    setModel(tableModel);

    table->horizontalHeader()->setDefaultSectionSize(100);
    table->setColumnWidth(TablespaceQuotasModel::TablespaceName, 200);
    table->setEditTriggers(QAbstractItemView::AllEditTriggers);

    DbItemListDelegate *tablespaceNameDelegate=new DbItemListDelegate("", this->queryScheduler,
                                                                      "get_tablespace_list", "tablespace",
                                                                      this, true);
    table->setItemDelegateForColumn(TablespaceQuotasModel::TablespaceName, tablespaceNameDelegate);

    BooleanDelegate *unlimitedDelegate=new BooleanDelegate(this, false);
    table->setItemDelegateForColumn(TablespaceQuotasModel::Unlimited, unlimitedDelegate);

    NumericDelegate *quotaDelegate=new NumericDelegate(this);
    table->setItemDelegateForColumn(TablespaceQuotasModel::Quota, quotaDelegate);

    IndexBasedComboBoxDelegate *quotaUnitDelegate=new IndexBasedComboBoxDelegate(QIcon(), DbUtil::getSizeUnitTexts(), this);
    table->setItemDelegateForColumn(TablespaceQuotasModel::Unit, quotaUnitDelegate);

    if(editMode){
        connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));

        tableModel->setColumnEnabled(0, false);

    }

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void TablespaceQuotasEditorTable::setRowData(int rowIx, TablespaceQuotasModel *model, TablespaceQuotaInfo *info)
{
    model->setData(model->index(rowIx, TablespaceQuotasModel::TablespaceName), info->tablespaceName);
    model->setData(model->index(rowIx, TablespaceQuotasModel::Unlimited), info->unlimitedQuota);
    model->setData(model->index(rowIx, TablespaceQuotasModel::Quota), info->quota);
    model->setData(model->index(rowIx, TablespaceQuotasModel::Unit), (int)info->quotaUnit, Qt::EditRole);
    QStringList sizeUnitTexts=DbUtil::getSizeUnitTexts();
    Q_ASSERT(sizeUnitTexts.size()>(int)info->quotaUnit);
    model->setData(model->index(rowIx, TablespaceQuotasModel::Unit), sizeUnitTexts.at((int)info->quotaUnit), Qt::DisplayRole);
}
