#include "datatableandtoolbarwidget.h"
#include "widgets/tabletoolbar.h"
#include "widgets/datatable.h"
#include <QtGui>

DataTableAndToolBarWidget::DataTableAndToolBarWidget(QAbstractItemModel *model, Qt::Orientation toolbarOrientation, QWidget *parent) :
    QWidget(parent)
{
    QLayout *layout=0;

    model->insertRows(0, 1);

    dataTable=new DataTable();
    dataTable->setModel(model);
    tableToolBar=new TableToolbar(dataTable);
    tableToolBar->setOrientation(toolbarOrientation);

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
