#include "datacomparerresultstab.h"
#include "widgets/datatable.h"
#include "navtree/dbtreemodel.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include <QtGui>

DataComparerResultsTab::DataComparerResultsTab(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparerResultsTab(uiManager, parent)
{
}

DataTable *DataComparerResultsTab::createChangeListTable()
{
    DataTable *changeListTable=new DataTable();

    QStringList columnNames = getTableColumnNames();
    tableModel=new QStandardItemModel(0, columnNames.size(), this);
    tableModel->setHorizontalHeaderLabels(columnNames);
    changeListTable->setModel(tableModel);
    changeListTable->horizontalHeader()->setDefaultSectionSize(100);
    changeListTable->horizontalHeader()->resizeSection(0, 250);
    changeListTable->horizontalHeader()->resizeSection(1, 150);
    changeListTable->horizontalHeader()->setStretchLastSection(true);
    changeListTable->setSortingEnabled(true);
    changeListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    changeListTable->setUiManager(uiManager);

    return changeListTable;
}

void DataComparerResultsTab::setCurrentSchema(const QString &currentSchemaName)
{
    changeListTable->setObjectListMode(-1, 0, -1, currentSchemaName, "TABLE");
}

QStringList DataComparerResultsTab::getTableColumnNames() const
{
    return QStringList() <<
                            tr("Table name") <<
                            tr("Status") <<
                            tr("Inserts") <<
                            tr("Updates") <<
                            tr("Deletes");
}

void DataComparerResultsTab::compareInfoAvailable(const DataCompareInfo &info)
{
    if(!info.tableName.isEmpty()){

        int rowCount = tableModel->rowCount();
        QStandardItem* item = 0;

        if(rowCount > 0){
            item = tableModel->item(rowCount-1);
            if(item->text()!=info.tableName){
                item = 0;
            }
        }

        if(item==0){
            QPixmap tableIcon=IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(DbTreeModel::Tables));
            QList<QStandardItem*> rowValues =  QList<QStandardItem*>() <<
                                              new QStandardItem(tableIcon, info.tableName) <<
                                              new QStandardItem(info.newStatus) <<
                                              new QStandardItem(QString::number(info.inserts));
            if(tableModel->columnCount()>3){
                rowValues << new QStandardItem(QString::number(info.updates)) << new QStandardItem(QString::number(info.deletes));
            }
            tableModel->appendRow(rowValues);
        }else{
            int row=rowCount-1;
            if(!info.newStatus.isEmpty()){
                tableModel->item(row, 1)->setText(info.newStatus);
            }

            if(info.inserts>0){
                WidgetHelper::increaseValueAtPos(tableModel, row, 2, info.inserts);
            }

            if(tableModel->columnCount()>3){ //in descendant classes may be less
                if(info.updates>0){
                    WidgetHelper::increaseValueAtPos(tableModel, row, 3, info.updates);
                }

                if(info.deletes>0){
                    WidgetHelper::increaseValueAtPos(tableModel, row, 4, info.deletes);
                }
            }
        }
    }

    if(!info.dml.isEmpty()){
        this->addText(info.dml);
    }

    changeListTable->scrollToBottom();
}
