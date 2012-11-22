#ifndef DBOBJECTVIEWERTABS_H
#define DBOBJECTVIEWERTABS_H

#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class DbObjectViewerToolBar;
class DbObjectViewerGenericTab;
class QTabWidget;

class DbObjectViewerTabs : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectViewerTabs(const QString &schemaName,
                                const QString &objectName,
                                DbTreeModel::DbTreeNodeType itemType,
                                DbUiManager *uiManager,
                                QWidget *parent);

    virtual void createUi();
    virtual void createTabs()=0;
    void addTab(DbObjectViewerGenericTab *tab, const QIcon &icon, const QString &title);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    IQueryScheduler *scheduler() const;

    void setHasSpecBodySwitcher(bool hasSpecBodySwitcher);

signals:
    void specBodySwitchRequested();

public slots:
    void loadTabData(int index);
    void refreshInfo();

    void beforeLoadTabInfo();
    void afterLoadTabInfo();

protected:
    const QString schemaName;
    const QString objectName;
    DbTreeModel::DbTreeNodeType itemType;
    DbUiManager *uiManager;

    IQueryScheduler *queryScheduler;

    DbObjectViewerToolBar *toolbar;
    QTabWidget *tabWidget;

    QAction *refreshButton;
    QAction *progressBarAction;
    QAction *lastSeparatorBeforeProgressBar;

    QHash< QWidget*, QList<QAction*> > tabSpecificActions;

    void createToolbarButtons();
    void showTabSpecificActions(QWidget *currentTab);

    int currentJobCount;

    bool hasSpecBodySwitcher;
};

#endif // DBOBJECTVIEWERTABS_H
