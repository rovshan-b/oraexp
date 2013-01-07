#include "worksheetwidget.h"
#include "worksheetresultpane.h"
#include <QtGui>

QByteArray WorksheetWidget::splitterSizes;

WorksheetWidget::WorksheetWidget(QWidget *parent) :
    QWidget(parent),
    queryScheduler(0)
{
    splitter=new QSplitter(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    queryPane = new WorksheetQueryPane();
    resultPane = new WorksheetResultPane();
    splitter->addWidget(queryPane);
    splitter->addWidget(resultPane);
    resultPane->setVisible(false);

    queryPane->setMinimumHeight(80);
    resultPane->setMinimumHeight(80);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);

    layout->setContentsMargins(0, 2, 0, 2);

    setLayout(layout);

    connect(queryPane, SIGNAL(queryDone(QueryResult)), this, SLOT(queryCompleted(QueryResult)));
    connect(queryPane, SIGNAL(message(QString)), this, SLOT(handleQueryPaneMessage(QString)));
}

void WorksheetWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    queryPane->setQueryScheduler(queryScheduler);
}

void WorksheetWidget::setContents(const QString &contents)
{
    queryPane->setContents(contents);
}

void WorksheetWidget::focusAvailable()
{
    queryPane->focusAvailable();
}

void WorksheetWidget::queryCompleted(const QueryResult &result)
{
    showResultPane();
    resultPane->displayQueryResults(this->queryScheduler, result);
}

void WorksheetWidget::splitterMoved()
{
    WorksheetWidget::splitterSizes = splitter->saveState();
}

void WorksheetWidget::handleQueryPaneMessage(const QString &msg)
{
    showResultPane();
    resultPane->displayMessage(msg);
}

void WorksheetWidget::showResultPane()
{
    if(!resultPane->isVisible()){
        setUpdatesEnabled(false);

        resultPane->setVisible(true);
        splitter->restoreState(WorksheetWidget::splitterSizes);

        setUpdatesEnabled(true);

        connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterMoved()));
    }
}
