#ifndef DATATABLEANDTOOLBARWIDGET_H
#define DATATABLEANDTOOLBARWIDGET_H

#include <QWidget>
#include "widgets/tabletoolbar.h"
#include "widgets/datatable.h"

class TableToolbar;
class DataTable;
class QAbstractItemModel;

class DataTableAndToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataTableAndToolBarWidget(QAbstractItemModel *model = 0, Qt::Orientation toolbarOrientation=Qt::Vertical, QWidget *parent = 0);

    TableToolbar *toolBar() const;
    DataTable *table() const;

    void setModel(QAbstractItemModel *model);

private:
    TableToolbar *tableToolBar;
    DataTable *dataTable;

};

#endif // DATATABLEANDTOOLBARWIDGET_H
