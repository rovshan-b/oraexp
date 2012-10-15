#include "datatableandtoolbarwidget.h"
#include <QtGui>

DataTableAndToolBarWidget::DataTableAndToolBarWidget(QAbstractItemModel *model,
                                                     Qt::Orientation toolbarOrientation,
                                                     QWidget *parent) :
    QWidget(parent)
{
    QLayout *layout=0;

    dataTable=new DataTable();
    tableToolBar=new TableToolbar(dataTable);
    tableToolBar->setEnabled(false);
    tableToolBar->setOrientation(toolbarOrientation);

    if(model){
        setModel(model);
    }

    if(toolbarOrientation==Qt::Vertical){
        layout=new QHBoxLayout();
        layout->addWidget(dataTable);
        layout->addWidget(tableToolBar);
    }else{
        layout=new QVBoxLayout();
        layout->addWidget(tableToolBar);
        layout->addWidget(dataTable);
    }

    layout->setContentsMargins(0,0,0,0);

    setLayout(layout);
}

TableToolbar *DataTableAndToolBarWidget::toolBar() const
{
    return this->tableToolBar;
}

DataTable *DataTableAndToolBarWidget::table() const
{
    return this->dataTable;
}

void DataTableAndToolBarWidget::setModel(QAbstractItemModel *model)
{
    Q_ASSERT(model);

    model->insertRows(0, 1);
    dataTable->setModel(model);
    tableToolBar->setEnabled(true);
}
