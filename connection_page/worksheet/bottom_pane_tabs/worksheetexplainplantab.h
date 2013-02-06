#ifndef WORKSHEETEXPLAINPLANTAB_H
#define WORKSHEETEXPLAINPLANTAB_H

#include "worksheetbottompanetab.h"
#include "connectivity/fetchresult.h"

class QTreeView;
class QStandardItem;
class ExplainPlanRow;
class QStackedWidget;
class QPlainTextEdit;
class DataTable;

class WorksheetExplainPlanTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetExplainPlanTab(QWidget *parent = 0);
    virtual ~WorksheetExplainPlanTab();
    
    virtual void createUi();
    virtual void addTabSpecificToolbarButtons();

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    void setStatementId(const QString &statementId);
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

    void setAutotraceMode(bool autotraceMode);

    static bool advancedOptionsVisible;
    static int explainPlanViewIndex;
    static int autotraceViewIndex;

private slots:
    void explainPlanQueryCompleted(const QueryResult &result);
    void explainPlanRecordFetched(const FetchResult &fetchResult);
    void explainPlanFetchCompleted(const QString &taskName, int resultsetIx);

    void planTableRecordFetched(const FetchResult &fetchResult);
    void planTableFetchCompleted(const QString &);

    void xplanRecordFetched(const FetchResult &fetchResult);
    void xplanFetchCompleted(const QString &);

    void showAdvancedOptions(bool show);
    void viewChangeRequested(QAction *action);

    void setCurrentView(int index);

    void currentViewChanged(int index);

private:
    QStackedWidget *stackedWidget;
    QTreeView *tree;
    QPlainTextEdit *textViewer;
    DataTable *sessionStatsTable;
    QString statementId;
    QHash<int, QStandardItem*> lastItemsForLevels;

    QList<ExplainPlanRow*> planData;
    QString xplanOutput;

    QAction *advancedOptionsAction;
    QAction *treeViewAction;
    QAction *xplanViewAction;
    QAction *statsViewAction;

    bool autotraceMode;

    void setupTree();
    void clearModel();

    void getExplainPlanData(IQueryScheduler *queryScheduler);
    
};

#endif // WORKSHEETEXPLAINPLANTAB_H
