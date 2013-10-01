#include "genericresultsetviewerdialog.h"
#include "widgets/datatable.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "codeeditor/codeeditor.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include <QtGui>

GenericResultsetViewerDialog::GenericResultsetViewerDialog(IQueryScheduler *queryScheduler,
                                                           const QString &query,
                                                           const QList<Param*> &params,
                                                           const QString &dbLinkName,
                                                           QWidget *parent,
                                                           const QPair<QString,QString> &iconColumn,
                                                           bool displayEditor) :
    QDialog(parent),
    queryScheduler(queryScheduler),
    editor(0),
    runQueryButton(0)
{
    QVBoxLayout *layout=new QVBoxLayout();

    QSplitter *splitter = new QSplitter(Qt::Vertical);


    table=new DataTable();
    splitter->addWidget(table);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);

    QString resQuery = query;
    setDbLinkName(resQuery, dbLinkName);
    table->setIconColumn(iconColumn.first, iconColumn.second);
    table->displayQueryResults(queryScheduler,
                               resQuery,
                               params);

    if(displayEditor){
        Q_ASSERT(params.size() == 0);

        editor = new CodeEditor();
        editor->setPlainText(resQuery);
        runQueryButton = new QPushButton(tr("Execute"));

        splitter->addWidget(WidgetHelper::nestWidgets(QList<QWidget*>() << editor << runQueryButton));
    }


    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    layout->addWidget(splitter);
    setLayout(layout);

    resize(600, 400);

    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(rowActivated(QModelIndex)));
    connect(runQueryButton, SIGNAL(clicked()), this, SLOT(executeQuery()));
}

void GenericResultsetViewerDialog::rowActivated(const QModelIndex &index)
{
    if(!index.isValid()){
        return;
    }

    selectedText = index.model()->index(index.row(), 0).data().toString();

    accept();
}

void GenericResultsetViewerDialog::executeQuery()
{
    if(queryScheduler->getDb()->isBusy()){
        runQueryButton->setText(tr("Connection is busy"));
        QTimer::singleShot(1000, this, SLOT(restoreQueryButtonText()));
        return;
    }

    table->displayQueryResults(queryScheduler,
                               editor->toPlainText(),
                               QList<Param*>());
}

void GenericResultsetViewerDialog::restoreQueryButtonText()
{
    runQueryButton->setText(tr("Execute"));
}
