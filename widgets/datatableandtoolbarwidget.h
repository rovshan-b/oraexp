#ifndef DATATABLEANDTOOLBARWIDGET_H
#define DATATABLEANDTOOLBARWIDGET_H

#include <QWidget>

class TableToolbar;
class DataTable;
class QAbstractItemModel;

class DataTableAndToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataTableAndToolBarWidget(QAbstractItemModel *model, Qt::Orientation toolbarOrientation=Qt::Vertical, QWidget *parent = 0);

    TableToolbar *toolBar() const;
    DataTable *table() const;

private:
    TableToolbar *tableToolBar;
    DataTable *dataTable;

};

#endif // DATATABLEANDTOOLBARWIDGET_H
