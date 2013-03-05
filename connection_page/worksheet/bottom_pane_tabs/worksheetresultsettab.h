#ifndef WORKSHEETRESULTSETTAB_H
#define WORKSHEETRESULTSETTAB_H

#include "worksheetbottompanetab.h"

class DbConnection;
class QStackedWidget;
class DataTable;
class Resultset;
class DataExportDialog;
class DataExporterBase;

class WorksheetResultsetTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetResultsetTab(QWidget *parent = 0);
    virtual ~WorksheetResultsetTab();

    virtual void addTabSpecificToolbarButtons();

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);
    void displayResultset(IQueryScheduler *queryScheduler, Resultset *rs);
private slots:
    void firstFetchCompleted();
    void exportData();

    void startExport(DataExporterBase *exporter);
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);

private:
    IQueryScheduler *queryScheduler;

    DataTable *resultsTable;
    QAction *dataExportAction;

    DataExportDialog *exportDialog;

    void setInProgress(bool inProgress);

};

#endif // WORKSHEETRESULTSETTAB_H
