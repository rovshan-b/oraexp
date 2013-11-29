#ifndef WORKSHEETRESULTPANE_H
#define WORKSHEETRESULTPANE_H

#include "widgets/subtabwidget.h"
#include "connectivity/queryresult.h"

class DbUiManager;
class IQueryScheduler;
class WorksheetBottomPaneTab;
class WorksheetQueryPane;

class WorksheetResultPane : public SubTabWidget
{

    Q_OBJECT

public:

    enum WorksheetBottomPaneTabType{
        InfoTab,
        ResultsetTab,
        ExplainPlanTab,
        AutotraceTab
    };

    explicit WorksheetResultPane(DbUiManager *uiManager, QWidget *parent=0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void displayQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result, WorksheetQueryPane *queryPane);
    void displayMessage(const QString &msg);

    void setResultsetTabNamesFromOutParams(Statement *stmt, QList<WorksheetBottomPaneTab *> tabs);

signals:
    void allTabsClosed();

public slots:
    void scriptModeStarted();
    void scriptModeCompleted();

private slots:
    void closeTab(int index);

private:
    WorksheetBottomPaneTab *getTabToDisplayResults(WorksheetBottomPaneTabType tabType);
    QList<WorksheetBottomPaneTab *> getTabsToDisplayResults(WorksheetBottomPaneTabType tabType,
                                                            int countToReturn,
                                                            int startIx = 0);

    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;

    int currentTabStartIx;

};

#endif // WORKSHEETRESULTPANE_H
