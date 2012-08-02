#ifndef GENERICRESULTSETVIEWERDIALOG_H
#define GENERICRESULTSETVIEWERDIALOG_H

#include <QDialog>

class DataTable;
class IQueryScheduler;

class GenericResultsetViewerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GenericResultsetViewerDialog(IQueryScheduler *queryScheduler, QWidget *parent);

private:
    DataTable *table;

};

#endif // GENERICRESULTSETVIEWERDIALOG_H
