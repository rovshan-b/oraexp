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
        ResultsetTab
    };

    explicit WorksheetResultPane(QWidget *parent=0);

    void displayQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

private:
    WorksheetBottomPaneTab *getTabToDisplayResults(WorksheetBottomPaneTabType tabType);
};

#endif // WORKSHEETRESULTPANE_H
