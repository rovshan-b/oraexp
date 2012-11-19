#ifndef DBOBJECTVIEWERWIDGET_H
#define DBOBJECTVIEWERWIDGET_H

#include "widgets/ondemandinfoviewerwidget.h"
#include "interfaces/iqueryscheduler.h"
#include "navtree/dbtreemodel.h"

class DbConnection;
class QToolBar;

class DbObjectViewerWidget : public OnDemandInfoViewerWidget
{
public:
    DbObjectViewerWidget(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);
    virtual void setObjectName(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType);

    virtual void createMainWidget(QLayout *) = 0;

    void createChildControls(DbTreeModel::DbTreeNodeType itemType,
                             DbUiManager *uiManager);
    bool areControlsCreated() const {return this->controlsCreated;}

protected:
    void createToolbarButtons(DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager);

    virtual void addSpecificToolbarButtons();

    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    QToolBar *toolbar;
    QAction *refreshButton;
    QAction *progressBarAction;

    bool controlsCreated;
};

#endif // DBOBJECTVIEWERWIDGET_H
