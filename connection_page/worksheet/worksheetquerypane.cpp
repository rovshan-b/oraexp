#include "worksheetquerypane.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/strutil.h"
#include "widgets/multieditorwidget.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "dialogs/bindparamsdialog.h"
#include "connectivity/statement.h"
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

    layout->addWidget(toolbar);
    layout->addWidget(multiEditor);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);

    setLayout(layout);

    WidgetHelper::updateActionTooltips(toolbar);

    multiEditor->getCurrentEditor()->editor()->setPlainText("select * from smpp_incoming where msg_id<:msg_id");
}

WorksheetQueryPane::~WorksheetQueryPane()
{
    qDeleteAll(paramHistory.values());
}

void WorksheetQueryPane::executeQuery(ExecuteMode executeMode)
{
    if(queryScheduler==0){
        return;
    }

    if(queryScheduler->getDb()->isBusy()){
        cout << "connection is busy" << endl;
        return;
    }

    QTextCursor textCursor;
    QString queryText=currentEditor()->editor()->getCurrentText(textCursor).trimmed();

    if(queryText.isEmpty()){
        emitMessage(tr("Query text cannot be empty"));
        return;
    }

    currentEditor()->editor()->pulsate(textCursor);

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
        queryText.prepend(QString("EXPLAIN PLAN SET STATEMENT_ID='%1' FOR ").arg(lastExpPlanStatementId));
    }

    QueryExecTask task;
    task.requester=this;
    task.taskName="execute_worksheet_query";
    task.query=queryText;
    task.params=params;
    task.queryCompletedSlotName="queryCompleted";

    queryScheduler->enqueueQuery(task);
    progressBarAction->setVisible(true);
}

void WorksheetQueryPane::executeExplainPlan()
{
    executeQuery(ExecuteExplainPlan);
}


void WorksheetQueryPane::queryCompleted(const QueryResult &result)
{
    if(result.hasError || lastExpPlanStatementId.isEmpty()){
        progressBarAction->setVisible(false);
        emit queryDone(result);
    }else{
        delete result.statement;
        queryScheduler->enqueueQuery("get_explain_plan_data",
                                     QList<Param*>() << new Param("statement_id", lastExpPlanStatementId),
                                     this,
                                     "get_explain_plan_data",
                                     "queryCompleted");
        lastExpPlanStatementId="";
    }
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
        default:
            break;
        }
        paramInfo->paramType=paramType;
        paramInfo->paramDirection=p->getParamDirection();
        if(paramType!=BindParamInfo::Cursor){
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
