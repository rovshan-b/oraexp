#include "worksheetresultpane.h"
#include "bottom_pane_tabs/worksheetinfotab.h"
#include "bottom_pane_tabs/worksheetresultsettab.h"
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
    WorksheetBottomPaneTab *tab=0;

    if(!result.hasError && result.statement!=0 && result.statement->rsCount()>0){
        tab=getTabToDisplayResults(ResultsetTab);
    }else{
        tab=getTabToDisplayResults(InfoTab);
    }

    tab->showQueryResults(queryScheduler, result);
    setCurrentWidget(tab);
}

WorksheetBottomPaneTab *WorksheetResultPane::getTabToDisplayResults(WorksheetBottomPaneTabType tabType)
{
    WorksheetBottomPaneTab *tab=0;

    //check whether current tab can be used
    int currentTabIx=currentIndex();
    if(currentTabIx!=-1){
        tab=static_cast<WorksheetBottomPaneTab*>(currentWidget());
        if(tab->getTabType()==tabType && !tab->isPinned()){
            return tab;
        }
    }

    //find first tab that can be used
    int tabCount=count();
    for(int i=0; i<tabCount; ++i){
        tab=static_cast<WorksheetBottomPaneTab*>(widget(i));
        if(tab->getTabType()==tabType && !tab->isPinned()){
            return tab;
        }
    }

    //no tab found, create new one
    switch(tabType){
    case ResultsetTab:
        tab=new WorksheetResultsetTab();
        addTab(tab, IconUtil::getIcon("table"), tr("Results"));
        break;
    case InfoTab:
        tab=new WorksheetInfoTab();
        addTab(tab, IconUtil::getIcon("worksheet"), tr("Info"));
        break;
    default: //should not happen
        tab=0;
        break;
    }

    return tab;
}
