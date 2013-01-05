#include "worksheetwidget.h"
#include "worksheetresultpane.h"
#include <QtGui>

WorksheetWidget::WorksheetWidget(QWidget *parent) :
    QWidget(parent),
    queryScheduler(0)
{
    QSplitter *splitter=new QSplitter(Qt::Vertical);
    queryPane = new WorksheetQueryPane();
    resultPane = new WorksheetResultPane();
    splitter->addWidget(queryPane);
    splitter->addWidget(resultPane);
    resultPane->setVisible(false);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);

    layout->setContentsMargins(0, 2, 0, 2);

    setLayout(layout);

    connect(queryPane, SIGNAL(queryDone(QueryResult)), this, SLOT(queryCompleted(QueryResult)));
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
    if(!resultPane->isVisible()){
        resultPane->setVisible(true);
    }
    resultPane->displayQueryResults(this->queryScheduler, result);
}
