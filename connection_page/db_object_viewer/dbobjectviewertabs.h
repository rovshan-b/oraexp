#ifndef DBOBJECTVIEWERTABS_H
#define DBOBJECTVIEWERTABS_H

#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class DbObjectViewerToolBar;
class DbObjectViewerWidget;
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
    void addTab(DbObjectViewerWidget *tab, const QIcon &icon, const QString &title);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    IQueryScheduler *scheduler() const;

signals:

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

};

#endif // DBOBJECTVIEWERTABS_H
