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
    toolbar->addAction(IconUtil::getIcon("explain_plan"), tr("Explain plan"), this, SLOT(executeExplainPlan()))->setShortcut(QKeySequence("Ctrl+E"));
    autotraceAction=toolbar->addAction(IconUtil::getIcon("autotrace"), tr("Enable auto trace"));
    autotraceAction->setCheckable(true);
    autotraceAction->setChecked(false);
    //autotraceAction->setShortcut(QKeySequence("Ctrl+B"));
    connect(autotraceAction, SIGNAL(triggered(bool)), this, SLOT(autotraceTriggeredByUser(bool)));

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

    multiEditor->getCurrentEditor()->editor()->setPlainText("select * from dt_test");
}

WorksheetQueryPane::~WorksheetQueryPane()
{
    qDeleteAll(paramHistory.values());
}

void WorksheetQueryPane::executeQuery(ExecuteMode executeMode)
{
    if(!canExecute()){
        return;
    }

    QString queryText=currentEditor()->editor()->getCurrentText(currentQueryCursor);

    if(queryText.isEmpty()){
        emitMessage(tr("Query text cannot be empty"));
        return;
    }

    currentEditor()->editor()->pulsate(currentQueryCursor);

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
    progressBarAction->setVisible(true);
}

void WorksheetQueryPane::executeExplainPlan()
{
    executeQuery(ExecuteExplainPlan);
}


void WorksheetQueryPane::queryCompleted(const QueryResult &result)
{
    progressBarAction->setVisible(false);

    if(result.hasError){ //reset error position according to editor
        QueryResult modifiedResult = highlightError(result);
        emit queryDone(modifiedResult);
    }else{
        emit queryDone(result);
    }
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

void WorksheetQueryPane::setAutotraceEnabled(bool enabled)
{
    autotraceAction->setChecked(enabled);
}

bool WorksheetQueryPane::isAutotraceEnabled() const
{
    return autotraceAction->isChecked();
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

QueryResult WorksheetQueryPane::highlightError(const QueryResult &result) //returns modified result with correct error position according to editor
{
    QueryResult modifiedResult=result;
    int modifiedPosition = -1;

    int errorPos = result.exception.getErrorPos();
    if(errorPos>0){
        int editorErrorPos = currentQueryCursor.selectionStart()+errorPos;
        if(!lastExpPlanStatementId.isEmpty()){ //this is an explain plan query
            editorErrorPos -= getExplainPlanPrefix().size();
        }
        CodeEditorUtil::highlightEditorError(currentEditor()->editor(), editorErrorPos, result.exception);

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
