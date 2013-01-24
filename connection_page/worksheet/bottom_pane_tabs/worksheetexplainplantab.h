#ifndef WORKSHEETEXPLAINPLANTAB_H
#define WORKSHEETEXPLAINPLANTAB_H

#include "worksheetbottompanetab.h"
#include "connectivity/fetchresult.h"

class QTreeView;
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
    void setStatementId(const QString &statementId);
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);

    static bool advancedOptionsVisible;

private slots:
    void explainPlanQueryCompleted(const QueryResult &result);
    void explainPlanRecordFetched(const FetchResult &fetchResult);
    void explainPlanFetchCompleted(const QString &);
    void showAdvancedOptions(bool show);

private:
    QTreeView *tree;
    QString statementId;
    QHash<int, QStandardItem*> lastItemsForLevels;

    QList<ExplainPlanRow*> planData;

    QAction *advancedOptionsAction;

    void setupTree();
    void clearModel();
    
};

#endif // WORKSHEETEXPLAINPLANTAB_H
