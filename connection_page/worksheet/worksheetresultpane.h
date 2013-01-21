#ifndef WORKSHEETRESULTPANE_H
#define WORKSHEETRESULTPANE_H

#include "widgets/subtabwidget.h"
#include "connectivity/queryresult.h"

class IQueryScheduler;
class WorksheetBottomPaneTab;

class WorksheetResultPane : public SubTabWidget
{

    Q_OBJECT

public:

    enum WorksheetBottomPaneTabType{
        InfoTab,
        ResultsetTab,
        ExplainPlanTab
    };

    explicit WorksheetResultPane(QWidget *parent=0);

    void displayQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);
    void displayMessage(const QString &msg);

    void setResultsetTabNamesFromOutParams(Statement *stmt, QList<WorksheetBottomPaneTab *> tabs);

private:
    WorksheetBottomPaneTab *getTabToDisplayResults(WorksheetBottomPaneTabType tabType);
    QList<WorksheetBottomPaneTab *> getTabsToDisplayResults(WorksheetBottomPaneTabType tabType, int countToReturn);
};

#endif // WORKSHEETRESULTPANE_H
