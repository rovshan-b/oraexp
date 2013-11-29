#ifndef WORKSHEETRESULTSETTAB_H
#define WORKSHEETRESULTSETTAB_H

#include "worksheetbottompanetab.h"

class DbConnection;
class DbUiManager;
class DataExporterThread;
class QStackedWidget;
class DataTable;
class Resultset;
class DataExportDialog;
class DataExporterBase;
class QLabel;
class DataTableEditController;
class Param;

class WorksheetResultsetTab : public WorksheetBottomPaneTab
{
    Q_OBJECT
public:
    explicit WorksheetResultsetTab(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~WorksheetResultsetTab();

    virtual void addTabSpecificToolbarButtons();

    virtual WorksheetResultPane::WorksheetBottomPaneTabType getTabType() const;
    virtual void showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result);
    void displayResultset(IQueryScheduler *queryScheduler, Statement *stmt, int rsIx);
private slots:
    void firstFetchCompleted();
    void exportData();
    void countRows();

    void startExport(DataExporterBase *exporter);
    void recordsExported(int count);
    void exportComplete();
    void exportError(const QString &errorMessage);
    void stopProgress();

    void reloadQuery();

    void cleanup();

private:
    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;
    DataExporterThread *exporterThread;

    DataTable *resultsTable;
    QAction *dataExportAction;
    QAction *countRowsAction;

    QLabel *statusBarLabel;
    QAction *labelAction;
    QAction *stopProgressButton;

    DataExportDialog *exportDialog;

    void setInProgress(bool inProgress, bool showsStatusMessage=false, bool cancellable=false);

    bool cursorClosed;

    DataTableEditController *editController;

    void makeEditable(bool editable);

    QString lastQuery;
    QList<Param*> lastParams;

};

#endif // WORKSHEETRESULTSETTAB_H
