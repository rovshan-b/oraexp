#include "genericresultsetviewerdialog.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "util/strutil.h"
#include <QtGui>

GenericResultsetViewerDialog::GenericResultsetViewerDialog(IQueryScheduler *queryScheduler,
                                                           const QString &query,
                                                           const QList<Param*> &params,
                                                           const QString &dbLinkName,
                                                           QWidget *parent,
                                                           const QPair<QString,QString> &iconColumn) :
    QDialog(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    table=new DataTable();
    layout->addWidget(table);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);

    QString resQuery = query;
    setDbLinkName(resQuery, dbLinkName);
    table->setIconColumn(iconColumn.first, iconColumn.second);
    table->displayQueryResults(queryScheduler,
                               resQuery,
                               params);

    setLayout(layout);

    resize(600, 400);

    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(rowActivated(QModelIndex)));
}

void GenericResultsetViewerDialog::rowActivated(const QModelIndex &index)
{
    if(!index.isValid()){
        return;
    }

    selectedText = index.model()->index(index.row(), 0).data().toString();

    accept();
}
