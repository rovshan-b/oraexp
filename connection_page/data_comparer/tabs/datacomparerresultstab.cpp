#include "datacomparerresultstab.h"
#include "widgets/datatable.h"
#include "navtree/dbtreemodel.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/dbutil.h"
#include <QtGui>

DataComparerResultsTab::DataComparerResultsTab(QWidget *parent) :
    DbObjectComparerResultsTab(parent)
{
}

DataTable *DataComparerResultsTab::createChangeListTable()
{
    DataTable *changeListTable=new DataTable();

    tableModel=new QStandardItemModel(0, 5, this);
    tableModel->setHorizontalHeaderLabels(QStringList() <<
                                          tr("Table name") <<
                                          tr("Status") <<
                                          tr("Inserts") <<
                                          tr("Updates") <<
                                          tr("Deletes"));
    changeListTable->setModel(tableModel);
    changeListTable->horizontalHeader()->setDefaultSectionSize(100);
    changeListTable->horizontalHeader()->resizeSection(0, 250);
    changeListTable->horizontalHeader()->resizeSection(1, 150);
    changeListTable->horizontalHeader()->setStretchLastSection(true);
    changeListTable->setSortingEnabled(true);
    changeListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    return changeListTable;
}

void DataComparerResultsTab::compareInfoAvailable(const DataCompareInfo &info)
{
    if(!info.tableName.isEmpty()){

        QList<QStandardItem*> items = tableModel->findItems(info.tableName, Qt::MatchFixedString|Qt::MatchCaseSensitive, 0);
        Q_ASSERT(items.size()<=1);

        if(items.size()==0){
            QPixmap tableIcon=IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType(DbTreeModel::Tables));
            tableModel->appendRow(QList<QStandardItem*>() <<
                                  new QStandardItem(tableIcon, info.tableName) <<
                                  new QStandardItem(info.newStatus) <<
                                  new QStandardItem(QString::number(info.inserts)) <<
                                  new QStandardItem(QString::number(info.updates)) <<
                                  new QStandardItem(QString::number(info.deletes)));
        }else{
            int row=items.at(0)->row();
            if(!info.newStatus.isEmpty()){
                tableModel->item(row, 1)->setText(info.newStatus);
            }

            if(info.inserts>0){
                WidgetHelper::increaseValueAtPos(tableModel, row, 2, info.inserts);
            }

            if(info.updates>0){
                WidgetHelper::increaseValueAtPos(tableModel, row, 3, info.updates);
            }

            if(info.deletes>0){
                WidgetHelper::increaseValueAtPos(tableModel, row, 4, info.deletes);
            }
        }
    }

    if(!info.dml.isEmpty()){
        this->addText(info.dml);
    }
}
