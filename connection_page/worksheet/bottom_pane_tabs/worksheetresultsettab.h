#ifndef WORKSHEETRESULTSETTAB_H
#define WORKSHEETRESULTSETTAB_H

#include "worksheetbottompanetab.h"

class DbConnection;
class DataTable;
class Resultset;

class WorksheetResultsetTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetResultsetTab(QWidget *parent = 0);

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);
    void displayResultset(IQueryScheduler *queryScheduler, Resultset *rs);

private slots:
    void firstFetchCompleted();

private:
    DataTable *resultsTable;

    QAction *progressBarAction;
};

#endif // WORKSHEETRESULTSETTAB_H
