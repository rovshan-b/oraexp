#include "worksheetwidget.h"
#include "worksheetresultpane.h"
#include <QtGui>

QByteArray WorksheetWidget::splitterSizes;

WorksheetWidget::WorksheetWidget(QWidget *parent) :
    QWidget(parent),
    queryScheduler(0),
    resultPaneShownBefore(false)
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
    connect(queryPane, SIGNAL(autotraceTriggered(bool)), this, SIGNAL(autotraceTriggered(bool)));

    connect(queryPane, SIGNAL(scriptModeStarted()), resultPane, SLOT(scriptModeStarted()));
    connect(queryPane, SIGNAL(scriptModeCompleted()), resultPane, SLOT(scriptModeCompleted()));

    connect(queryPane, SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));

    connect(resultPane, SIGNAL(allTabsClosed()), this, SLOT(hideResultPane()));
}

void WorksheetWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    queryPane->setQueryScheduler(queryScheduler);
    resultPane->setQueryScheduler(queryScheduler);
}

void WorksheetWidget::setContents(const QString &contents)
{
    queryPane->setContents(contents);
}

QString WorksheetWidget::getContents() const
{
    return queryPane->getContents();
}

void WorksheetWidget::insertText(const QString &text)
{
    queryPane->insertText(text);
}

void WorksheetWidget::focusAvailable()
{
    queryPane->focusAvailable();
}

void WorksheetWidget::setAutotraceEnabled(bool enabled)
{
    queryPane->setAutotraceEnabled(enabled);
}

bool WorksheetWidget::isAutotraceEnabled() const
{
    return queryPane->isAutotraceEnabled();
}

bool WorksheetWidget::isModified() const
{
    return queryPane->isModified();
}

void WorksheetWidget::setModified(bool modified)
{
    queryPane->setModified(modified);
}

QTextDocument *WorksheetWidget::getTextDocument()
{
    return queryPane->getTextDocument();
}

void WorksheetWidget::queryCompleted(const QueryResult &result)
{
    showResultPane();
    resultPane->displayQueryResults(this->queryScheduler, result, queryPane);
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
        if(!resultPaneShownBefore){
            resultPaneShownBefore = true;

            setUpdatesEnabled(false);

            resultPane->setVisible(true);
            splitter->restoreState(WorksheetWidget::splitterSizes);

            setUpdatesEnabled(true);

            connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterMoved()));
        }else{
            resultPane->setVisible(true);
        }
    }
}

void WorksheetWidget::hideResultPane()
{
    resultPane->setVisible(false);
}
