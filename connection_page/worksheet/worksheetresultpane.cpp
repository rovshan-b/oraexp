#include "worksheetresultpane.h"
#include "bottom_pane_tabs/worksheetinfotab.h"
#include "bottom_pane_tabs/worksheetresultsettab.h"
#include "bottom_pane_tabs/worksheetexplainplantab.h"
#include "util/iconutil.h"
#include "connectivity/statement.h"
#include <QtGui>


WorksheetResultPane::WorksheetResultPane(QWidget *parent) : SubTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
}

void WorksheetResultPane::displayQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result)
{
    int rsCount=0;
    if(result.statement!=0){
        rsCount = result.statement->rsCount();
    }

    OraExp::QueryType statementType = result.hasError ? OraExp::QueryTypeUnknown : result.statement->getStatementType();
    bool isExplainPlan = result.taskName=="get_explain_plan_data";

    if((result.hasError) || (result.statement!=0 && statementType!=OraExp::QueryTypeSelect && !isExplainPlan)){
        WorksheetBottomPaneTab *tab=getTabToDisplayResults(InfoTab);
        tab->showQueryResults(queryScheduler, result);

        if(rsCount!=1){
            setCurrentWidget(tab);
        }
    }

    if(result.statement!=0 && result.statement->rsCount()>0 && !isExplainPlan){
        QList<WorksheetBottomPaneTab *> tabs = getTabsToDisplayResults(ResultsetTab, rsCount);
        if(statementType==OraExp::QueryTypeDeclare || statementType==OraExp::QueryTypeBegin){
            setResultsetTabNamesFromOutParams(result.statement, tabs);
        }
        for(int i=0; i<tabs.size(); ++i){
            WorksheetResultsetTab *rsTab = static_cast<WorksheetResultsetTab *>(tabs.at(i));
            rsTab->displayResultset(queryScheduler, result.statement->rsAt(i));

            if(rsCount==1){
                if(statementType!=OraExp::QueryTypeDeclare && statementType!=OraExp::QueryTypeBegin){
                    setTabText(indexOf(rsTab), tr("Results"));
                }
                setCurrentWidget(rsTab);
            }
        }
    }

    if(isExplainPlan){
        WorksheetBottomPaneTab *tab=getTabToDisplayResults(ExplainPlanTab);
        tab->showQueryResults(queryScheduler, result);
        setCurrentWidget(tab);
    }
}

void WorksheetResultPane::displayMessage(const QString &msg)
{
    WorksheetBottomPaneTab *tab=getTabToDisplayResults(InfoTab);
    WorksheetInfoTab *infoTab = qobject_cast<WorksheetInfoTab*>(tab);
    Q_ASSERT(infoTab);

    infoTab->showMessage(msg);

    setCurrentWidget(tab);
}

void WorksheetResultPane::setResultsetTabNamesFromOutParams(Statement *stmt, QList<WorksheetBottomPaneTab *> tabs)
{
    int rsParamIx=0;
    for(int i=0; i<stmt->paramCount(); ++i){
        Param *param=stmt->param(i);
        if(param->getParamType()==Param::Stmt){
            setTabText(indexOf(tabs.at(rsParamIx++)), param->getParamName());
        }
    }
}

WorksheetBottomPaneTab *WorksheetResultPane::getTabToDisplayResults(WorksheetBottomPaneTabType tabType)
{
    return getTabsToDisplayResults(tabType, 1).at(0);
}

QList<WorksheetBottomPaneTab *> WorksheetResultPane::getTabsToDisplayResults(WorksheetResultPane::WorksheetBottomPaneTabType tabType, int countToReturn)
{
    QList<WorksheetBottomPaneTab *> tabs;
    WorksheetBottomPaneTab *tab;

    //check whether current tab can be used
    int currentTabIx=currentIndex();
    if(currentTabIx!=-1){
        tab=static_cast<WorksheetBottomPaneTab*>(currentWidget());
        if(tab->getTabType()==tabType && !tab->isPinned()){
            tabs.append(tab);
        }
    }

    if(tabs.size()==countToReturn){
        return tabs;
    }

    //find first tab that can be used
    int tabCount=count();
    for(int i=0; i<tabCount; ++i){
        tab=static_cast<WorksheetBottomPaneTab*>(widget(i));
        if(tab->getTabType()==tabType && !tab->isPinned() && !tabs.contains(tab)){
            tabs.append(tab);
        }

        if(tabs.size()==countToReturn){
            break;
        }
    }

    int foundCount = tabs.size();

    for(int i=foundCount; i<countToReturn; ++i){
        switch(tabType){
        case ResultsetTab:
            tab=new WorksheetResultsetTab();
            addTab(tab, IconUtil::getIcon("table"), tr("Results"));
            tabs.append(tab);
            break;
        case InfoTab:
            tab=new WorksheetInfoTab();
            addTab(tab, IconUtil::getIcon("worksheet"), tr("Info"));
            tabs.append(tab);
            break;
        case ExplainPlanTab:
            tab=new WorksheetExplainPlanTab();
            addTab(tab, IconUtil::getIcon("explain_plan"), tr("Explain plan"));
            tabs.append(tab);
            break;
        default: //should not happen
            tab=0;
            Q_ASSERT(false);
            break;
        }

        if(tab!=0){
            tab->createUi();
        }
    }

    Q_ASSERT(tabs.size()==countToReturn);

    return tabs;
}
