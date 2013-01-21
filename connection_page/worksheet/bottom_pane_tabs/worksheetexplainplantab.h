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
    
    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

private slots:
    void recordsFetched(const QList<QStringList> &records);
    void fetchComplete();
    void fetchError(const OciException &ex);

private:
    QTreeView *tree;
    RecordFetcherThread *fetcherThread;
    IQueryScheduler *queryScheduler;
    Statement *currStatement;

    QHash<int, QStandardItem*> lastItemsForLevels;

    QList<ExplainPlanRow*> planData;

    void setupTree();

    void startFetcherThread();
    void deleteFetcherThread();

    void clearModel();
    
};

#endif // WORKSHEETEXPLAINPLANTAB_H
