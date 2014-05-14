#include "worksheetquerypane.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/strutil.h"
#include "util/codeeditorutil.h"
#include "widgets/multieditorwidget.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "dialogs/bindparamsdialog.h"
#include "connectivity/statement.h"
#include "errors.h"
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
    toolbar->addAction(IconUtil::getIcon("execute_as_script"), tr("Execute as script"), this, SLOT(executeAsScript()))->setShortcut(QKeySequence("F9"));
    //toolbar->addSeparator();
    toolbar->addAction(IconUtil::getIcon("explain_plan"), tr("Explain plan"), this, SLOT(executeExplainPlan()))->setShortcut(QKeySequence("Ctrl+E"));
    autotraceAction=toolbar->addAction(IconUtil::getIcon("autotrace"), tr("Enable auto trace"));
    autotraceAction->setCheckable(true);
    autotraceAction->setChecked(false);
    //autotraceAction->setShortcut(QKeySequence("Ctrl+B"));
    connect(autotraceAction, SIGNAL(triggered(bool)), this, SLOT(autotraceTriggeredByUser(bool)));

    toolbar->addSeparator();

    toolbar->addAction(IconUtil::getIcon("commit"), tr("Commit"), this, SLOT(commit()));
    toolbar->addAction(IconUtil::getIcon("rollback"), tr("Rollback"), this, SLOT(rollback()));

    toolbar->addSeparator();

    timerLabel = new QLabel();
    toolbar->addWidget(timerLabel);
    resetProgressLabel();

    QProgressBar *progressBar=new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setMaximumWidth(100);
    progressBar->setMaximumHeight(16);
    progressBarAction=toolbar->addWidget(progressBar);
    progressBarAction->setVisible(false);

    stopProgressAction = toolbar->addAction(IconUtil::getIcon("stop"), tr("Stop"), this, SLOT(stopCurrentQuery()));
    stopProgressAction->setVisible(false);

    //create code editor
    multiEditor = new MultiEditorWidget();

    WidgetHelper::addStretchToToolbar(toolbar);

    QLabel *infoLabel=multiEditor->createInfoLabel();
    WidgetHelper::changeFontSize(infoLabel, -1);
    multiEditor->setInfoLabelTextFormat(QString("Line:%1 Pos:%2 (%3)     "));
    toolbar->addWidget(infoLabel);

    multiEditor->addSplittingActions(toolbar);

    layout->addWidget(toolbar);
    layout->addWidget(multiEditor);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);

    setLayout(layout);

    WidgetHelper::updateActionTooltips(toolbar);

    connect(&sequentialRunner, SIGNAL(beforeExecute(QString,int,int)), this, SLOT(beforeExecuteSequentialQuery(QString,int,int)));
    connect(&sequentialRunner, SIGNAL(queryResultAvailable(QueryResult)), this, SLOT(sequentialQueryCompleted(QueryResult)));
    connect(&sequentialRunner, SIGNAL(completed(bool)), this, SLOT(sequentialExecutionCompleted()));

    connect(multiEditor->getCurrentEditor()->editor()->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

WorksheetQueryPane::~WorksheetQueryPane()
{
    qDeleteAll(paramHistory.values());
}

void WorksheetQueryPane::executeQuery(ExecuteMode executeMode, const QString &textToExecute)
{
    if(!canExecute()){
        return;
    }

    QString queryText=textToExecute.isEmpty() ? currentEditor()->editor()->getCurrentText(currentQueryCursor) : textToExecute;

    if(queryText.isEmpty()){
        emitMessage(tr("Query text cannot be empty"));
        return;
    }

    if(textToExecute.isEmpty()){
        currentEditor()->editor()->pulsate(currentQueryCursor);
    }else{
        currentQueryCursor = QTextCursor();
    }

    QList<Param*> params;

    if(executeMode==ExecuteQuery){
        lastExpPlanStatementId="";
        QList<BindParamInfo::BindParamType> suggestedParamTypes;
        QStringList bindParams = PlSqlParseHelper::getBindParams(queryText, &suggestedParamTypes);
        if(bindParams.size()>0){
            params=promptForBindParams(bindParams, suggestedParamTypes);
            //param count will be 0 if user pressed cancel
            if(params.size()==0){
                return;
            }
        }
    }else if(executeMode==ExecuteExplainPlan){
        lastExpPlanStatementId=md5(QUuid::createUuid().toString()).left(30);
        queryText.prepend(getExplainPlanPrefix());
    }

    QueryExecTask task;
    task.requester=this;
    task.taskName="execute_worksheet_query";
    task.query=queryText;
    task.params=params;
    task.queryCompletedSlotName="queryCompleted";

    this->currentQuery = queryText;

    queryScheduler->enqueueQuery(task);
    setInProgress(true);
}

void WorksheetQueryPane::executeAsScript()
{
    if(!canExecute()){
        return;
    }

    CodeEditor *editor = currentEditor()->editor();
    QTextCursor cur;
    QString queryText = editor->getCurrentText(cur, true);

    sequentialRunnerStartPos = qMin(cur.position(), cur.anchor());

    if(queryText.trimmed().isEmpty()){
        emitMessage(tr("Query text cannot be empty"));
        return;
    }

    this->currentQueryCursor = cur;

    emit scriptModeStarted();

    multiEditor->setReadOnly(true);
    setInProgress(true);

    sequentialRunner.execute(queryText, this);
}

void WorksheetQueryPane::executeExplainPlan()
{
    executeQuery(ExecuteExplainPlan);
}

void WorksheetQueryPane::commit()
{
    executeQuery(ExecuteQuery, "COMMIT");
}

void WorksheetQueryPane::rollback()
{
    executeQuery(ExecuteQuery, "ROLLBACK");
}

void WorksheetQueryPane::handleQueryCompleted(const QueryResult &result, int queryStartPos)
{
    if(result.hasError && !currentQueryCursor.isNull()){ //reset error position according to editor
        QueryResult modifiedResult = highlightError(result, queryStartPos, sequentialRunner.isBusy());
        emit queryDone(modifiedResult);
    }else{
        emit queryDone(result);
    }
}

void WorksheetQueryPane::queryCompleted(const QueryResult &result)
{
    setInProgress(false);

    handleQueryCompleted(result, qMin(currentQueryCursor.position(), currentQueryCursor.anchor()));
}

void WorksheetQueryPane::beforeExecuteSequentialQuery(const QString &query, int startPos, int endPos)
{
    Q_UNUSED(endPos);


    this->currentQuery = query;

    CodeEditor *editor = currentEditor()->editor();
    CodeEditorUtil::markPosition(editor, sequentialRunnerStartPos+startPos);
}

void WorksheetQueryPane::sequentialQueryCompleted(const QueryResult &result)
{
    handleQueryCompleted(result, sequentialRunnerStartPos+sequentialRunner.getCurrentQueryPos());
}

void WorksheetQueryPane::sequentialExecutionCompleted()
{
    currentEditor()->editor()->setMarkedLine(-1);
    setInProgress(false);
    multiEditor->setReadOnly(false);

    emit scriptModeCompleted();
}

void WorksheetQueryPane::stopCurrentQuery()
{
    if(sequentialRunner.isBusy()){
        sequentialRunner.stop();
        stopProgressAction->setVisible(false);
    }

    queryScheduler->getDb()->breakCurrentQuery();
}

void WorksheetQueryPane::autotraceTriggeredByUser(bool checked)
{
    if(!canExecute()){
        return;
    }

    queryScheduler->enqueueQuery(checked ? "enable_autotrace" : "disable_autotrace",
                                 QList<Param*>(), this, checked ? "enable_autotrace" : "disable_autotrace", "autotraceQueryCompleted");
}

void WorksheetQueryPane::autotraceQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        autotraceAction->setChecked(!autotraceAction->isChecked());

        QueryResult modifiedResult=result;
        if(modifiedResult.exception.getErrorCode()==ERR_INSUFFICIENT_PRIVILEGES){
            modifiedResult.exception.addToErrorMessage(tr("You need ALTER SESSION privilege to use autotrace feature.\n"), false);
        }

        emit queryDone(modifiedResult);
        return;
    }

    delete result.statement;

    emit autotraceTriggered(autotraceAction->isChecked());
}

void WorksheetQueryPane::setContents(const QString &contents)
{
    //currentEditor()->editor()->setPlainText(contents);
    multiEditor->setInitialText(contents);
}

QString WorksheetQueryPane::getContents() const
{
    return currentEditor()->editor()->toPlainText();
}

void WorksheetQueryPane::insertText(const QString &text)
{
    currentEditor()->editor()->insertPlainText(text);
    currentEditor()->editor()->setFocus();
}

void WorksheetQueryPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
    sequentialRunner.setQueryScheduler(queryScheduler);
    multiEditor->setQueryScheduler(queryScheduler);

    toolbar->setEnabled(true);
}

CodeEditorAndSearchPaneWidget *WorksheetQueryPane::currentEditor() const
{
    return multiEditor->getCurrentEditor();
}

void WorksheetQueryPane::focusAvailable()
{
    currentEditor()->editor()->setFocus();
}

void WorksheetQueryPane::setAutotraceEnabled(bool enabled)
{
    autotraceAction->setChecked(enabled);
}

bool WorksheetQueryPane::isAutotraceEnabled() const
{
    return autotraceAction->isChecked();
}

bool WorksheetQueryPane::isInScriptMode() const
{
    return sequentialRunner.isBusy();
}

bool WorksheetQueryPane::isModified() const
{
    return multiEditor->getCurrentEditor()->editor()->document()->isModified();
}

void WorksheetQueryPane::setModified(bool modified)
{
    currentEditor()->editor()->document()->setModified(modified);
}

QTextDocument *WorksheetQueryPane::getTextDocument() const
{
    return currentEditor()->getTextDocument();
}

void WorksheetQueryPane::emitMessage(const QString &msg)
{
    emit message(msg);
}

QList<Param*> WorksheetQueryPane::promptForBindParams(const QStringList &bindParams, const QList<BindParamInfo::BindParamType> &suggestedParamTypes)
{
    QList<Param*> params;

    BindParamsDialog dialog(bindParams, suggestedParamTypes, paramHistory, this);
    if(dialog.exec()){
        params = dialog.getParams();
        saveBindParams(params);
    }

    return params;
}

void WorksheetQueryPane::saveBindParams(const QList<Param *> &params)
{
    foreach(Param* p, params){
        BindParamInfo *paramInfo=paramHistory.value(p->getParamName());
        if(paramInfo==0){
            paramInfo = new BindParamInfo();
            paramHistory[p->getParamName()]=paramInfo;
        }
        BindParamInfo::BindParamType paramType=BindParamInfo::StringOrNumber;
        switch(p->getParamType()){
        case Param::Datetime:
            paramType=BindParamInfo::Date;
            break;
        case Param::Stmt:
            paramType=BindParamInfo::Cursor;
            break;
        case Param::ReturningInto:
            paramType=BindParamInfo::ReturningInto;
            break;
        default:
            break;
        }
        paramInfo->paramType=paramType;
        paramInfo->paramDirection=p->getParamDirection();
        if(paramType!=BindParamInfo::Cursor && paramType!=BindParamInfo::ReturningInto){
            QString value = p->toString();
            int valueIx = paramInfo->paramValueHistory.indexOf(value);
            if(valueIx==-1){
                paramInfo->paramValueHistory.append(value);
            }else if(valueIx!=paramInfo->paramValueHistory.size()-1){ //if == then value already on top, do nothing
                paramInfo->paramValueHistory.removeAt(valueIx);
                paramInfo->paramValueHistory.append(value);
            }
        }
    }
}

bool WorksheetQueryPane::canExecute()
{
    if(queryScheduler==0){
        return false;
    }

    if(queryScheduler->getDb()->isBusy()){
        emitMessage(tr("Connection is busy"));
        return false;
    }

    return true;
}

QString WorksheetQueryPane::getExplainPlanPrefix() const
{
    return QString("EXPLAIN PLAN SET STATEMENT_ID='%1' FOR ").arg(lastExpPlanStatementId);
}

QueryResult WorksheetQueryPane::highlightError(const QueryResult &result, int queryStartPos, bool append) //returns modified result with correct error position according to editor
{
    QueryResult modifiedResult=result;
    int modifiedPosition = -1;

    int errorPos = result.exception.getErrorPos();
    if(errorPos>0){
        int editorErrorPos = queryStartPos+errorPos;
        if(!lastExpPlanStatementId.isEmpty()){ //this is an explain plan query
            editorErrorPos -= getExplainPlanPrefix().size();
        }
        CodeEditorUtil::highlightEditorError(currentEditor()->editor(),
                                             editorErrorPos,
                                             result.exception,
                                             append);

        modifiedPosition=editorErrorPos;
        modifiedResult.exception.setErrorPos(modifiedPosition);
        QTextCursor cur=currentEditor()->editor()->textCursor();
        cur.setPosition(editorErrorPos);
        modifiedResult.exception.addToErrorMessage(QString("Line: %1 Pos: %2 (%3)").
                                                   arg(cur.blockNumber()+1).
                                                   arg(cur.positionInBlock()+1).
                                                   arg(cur.position()+1), false);

        qDebug() << "moved error position from" << errorPos << "to" << modifiedPosition;
    }

    return modifiedResult;
}

void WorksheetQueryPane::setInProgress(bool progress)
{
    if(progress){
        progressTimerId = startTimer(1000);
        progressTimer.start();
    }else{
        killTimer(progressTimerId);
        updateProgressLabel();

        progressBarAction->setVisible(false);
        stopProgressAction->setVisible(false);
    }
}

void WorksheetQueryPane::resetProgressLabel()
{
    timerLabel->setText(" 00:00:00.00   ");
}

void WorksheetQueryPane::updateProgressLabel()
{
    if(!progressBarAction->isVisible()){
        progressBarAction->setVisible(true);
        stopProgressAction->setVisible(true);
    }

    timerLabel->setText(QString(" %1   ").arg(formatMsecs(progressTimer.elapsed(), true)));
}

void WorksheetQueryPane::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==progressTimerId){
        updateProgressLabel();
    }
}
