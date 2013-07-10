#ifndef GENERICQUERYVIEWERTABS_H
#define GENERICQUERYVIEWERTABS_H

#include <QWidget>
#include <QHash>

class DbUiManager;
class IQueryScheduler;
class ToolBar;
class QTabWidget;
class GenericQueryViewerWidget;

class GenericQueryViewerTabs : public QWidget
{
    Q_OBJECT
public:
    explicit GenericQueryViewerTabs(DbUiManager *uiManager,
                                    QWidget *parent);

    virtual void createUi();
    virtual void createTabs()=0;
    void addTab(GenericQueryViewerWidget *tab, const QIcon &icon, const QString &title);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    IQueryScheduler *scheduler() const;
    
public slots:
    void loadTabData(int index);
    void refreshInfo();

    void beforeLoadTabInfo();
    void afterLoadTabInfo();

protected:
    DbUiManager *uiManager;
    IQueryScheduler *queryScheduler;

    ToolBar *toolbar;
    QTabWidget *tabWidget;

    QAction *refreshButton;
    QAction *progressBarAction;
    QAction *lastSeparatorBeforeProgressBar;

    QHash< QWidget*, QList<QAction*> > tabSpecificActions;

    virtual void initTab(GenericQueryViewerWidget *tab);

    virtual void createToolbarButtons();
    void createRefreshButton();
    void createProgressBarAction();

    void showTabSpecificActions(QWidget *currentTab);

    int currentJobCount;

private:
    bool needsRefresh;
    
};

#endif // GENERICQUERYVIEWERTABS_H
