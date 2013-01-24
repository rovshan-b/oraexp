#ifndef WORKSHEETEXPLAINPLANTAB_H
#define WORKSHEETEXPLAINPLANTAB_H

#include "worksheetbottompanetab.h"

class QTreeView;
class RecordFetcherThread;
class QStandardItem;
class ExplainPlanRow;

class WorksheetExplainPlanTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetExplainPlanTab(QWidget *parent = 0);
    virtual ~WorksheetExplainPlanTab();
    
    virtual void createUi();
    virtual void addTabSpecificToolbarButtons();

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

    static bool advancedOptionsVisible;

private slots:
    void recordsFetched(const QList<QStringList> &records);
    void fetchComplete();
    void fetchError(const OciException &ex);
    void showAdvancedOptions(bool show);

private:
    QTreeView *tree;
    RecordFetcherThread *fetcherThread;
    IQueryScheduler *queryScheduler;
    Statement *currStatement;

    QHash<int, QStandardItem*> lastItemsForLevels;

    QList<ExplainPlanRow*> planData;

    QAction *advancedOptionsAction;

    void setupTree();

    void startFetcherThread();
    void deleteFetcherThread();

    void clearModel();
    
};

#endif // WORKSHEETEXPLAINPLANTAB_H
