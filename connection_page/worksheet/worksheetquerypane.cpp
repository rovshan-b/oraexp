#include "worksheetquerypane.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

#include <iostream>
using namespace std;

WorksheetQueryPane::WorksheetQueryPane(QWidget *parent) :
    QWidget(parent), db(0)
{
    QVBoxLayout *layout=new QVBoxLayout();

    //create toolbar
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setEnabled(false);

    QToolButton *executeButton=new QToolButton();
    executeButton->setIcon(QIcon(":/images/execute.png"));
    executeButton->setAutoRaise(true);
    executeButton->setToolTip(tr("Execute query"));
    executeButton->setShortcut(QKeySequence("Ctrl+Return"));
    connect(executeButton, SIGNAL(clicked()), this, SLOT(executeQuery()));

    toolbar->addWidget(executeButton);

    toolbar->addSeparator();

    QProgressBar *progressBar=new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setMaximumWidth(100);
    progressBar->setMaximumHeight(16);
    progressBarAction=toolbar->addWidget(progressBar);
    progressBarAction->setVisible(false);

    //create code editor
    codeEditorAndSearchPaneWidget=new CodeEditorAndSearchPaneWidget();
    codeEditor=codeEditorAndSearchPaneWidget->editor();
    codeEditor->setPlainText("select * from az_fdl");

    layout->addWidget(toolbar);
    layout->addWidget(codeEditorAndSearchPaneWidget);

    layout->setContentsMargins(2,0,0,0);
    layout->setSpacing(5);

    setLayout(layout);
}

void WorksheetQueryPane::setConnection(DbConnection *db)
{
    this->db=db;
    toolbar->setEnabled(true);
}

void WorksheetQueryPane::executeQuery()
{
    if(db==0){
        return;
    }

    if(db->isBusy()){
        cout << "connection is busy" << endl;
        return;
    }

    QueryExecTask task;
    task.requester=this;
    task.taskName="execute_worksheet_query";
    task.query=codeEditor->getCurrentText();
    task.queryCompletedSlotName="queryCompleted";

    db->enqueueQuery(task);
    progressBarAction->setVisible(true);
}


void WorksheetQueryPane::queryCompleted(const QueryResult &result)
{
    progressBarAction->setVisible(false);

    emit queryDone(result);
}

void WorksheetQueryPane::setContents(const QString &contents)
{
    codeEditor->setPlainText(contents);
}
