#include "genericresultsetviewerdialog.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

GenericResultsetViewerDialog::GenericResultsetViewerDialog(IQueryScheduler *queryScheduler, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    table=new DataTable();
    layout->addWidget(table);

    //table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    //QueryResult result=queryScheduler->getDb()->executeQuery("select * from user_tablespaces");
    //table->setResultset(queryScheduler, result.statement->rsAt(0));

    setLayout(layout);

    resize(600, 400);
}
