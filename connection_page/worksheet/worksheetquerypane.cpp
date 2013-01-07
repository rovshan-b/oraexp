#include "worksheetquerypane.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "widgets/multieditorwidget.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include <QtGui>

#include <iostream>
using namespace std;

WorksheetQueryPane::WorksheetQueryPane(QWidget *parent) :
    QWidget(parent), queryScheduler(0)
{
    QVBoxLayout *layout=new QVBoxLayout();

    //create toolbar
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setEnabled(false);

    toolbar->addAction(IconUtil::getIcon("execute"), tr("Execute"), this, SLOT(executeQuery()))->setShortcut(QKeySequence("Ctrl+Return"));

    toolbar->addSeparator();

    QProgressBar *progressBar=new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setMaximumWidth(100);
    progressBar->setMaximumHeight(16);
    progressBarAction=toolbar->addWidget(progressBar);
    progressBarAction->setVisible(false);

    //create code editor
    multiEditor = new MultiEditorWidget();
    WidgetHelper::addStretchToToolbar(toolbar);
    multiEditor->addSplittingActions(toolbar);

    WidgetHelper::updateActionTooltips(toolbar);
    layout->addWidget(toolbar);
    layout->addWidget(multiEditor);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);

    setLayout(layout);

    WidgetHelper::updateActionTooltips(toolbar);

    multiEditor->getCurrentEditor()->editor()->setPlainText("select * from smpp_incoming");
}

void WorksheetQueryPane::executeQuery()
{
    if(queryScheduler==0){
        return;
    }

    if(queryScheduler->getDb()->isBusy()){
        cout << "connection is busy" << endl;
        return;
    }

    QString queryText=currentEditor()->editor()->getCurrentText().trimmed();

    if(queryText.isEmpty()){
        emitMessage(tr("Query text cannot be empty"));
        return;
    }

    QStringList bindParams = PlSqlParseHelper::getBindParams(queryText);

    QueryExecTask task;
    task.requester=this;
    task.taskName="execute_worksheet_query";
    task.query=queryText;
    task.queryCompletedSlotName="queryCompleted";

    queryScheduler->enqueueQuery(task);
    progressBarAction->setVisible(true);
}


void WorksheetQueryPane::queryCompleted(const QueryResult &result)
{
    progressBarAction->setVisible(false);

    emit queryDone(result);
}

void WorksheetQueryPane::setContents(const QString &contents)
{
    currentEditor()->editor()->setPlainText(contents);
}

void WorksheetQueryPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    toolbar->setEnabled(true);
}

void WorksheetQueryPane::showSearchWidget()
{
    currentEditor()->showSearchPane();
}

void WorksheetQueryPane::findNext()
{
    currentEditor()->findNext();
}

void WorksheetQueryPane::findPrevious()
{
    currentEditor()->findPrevious();
}

CodeEditorAndSearchPaneWidget *WorksheetQueryPane::currentEditor() const
{
    return multiEditor->getCurrentEditor();
}

void WorksheetQueryPane::focusAvailable()
{
    currentEditor()->editor()->setFocus();
}

void WorksheetQueryPane::emitMessage(const QString &msg)
{
    emit message(msg);
}
