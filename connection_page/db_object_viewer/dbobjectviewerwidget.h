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

    void createChildControls();
    bool areControlsCreated() const {return this->controlsCreated;}

    virtual QList<QAction*> getSpecificToolbarButtons();

protected:
    void createToolbarButtons(DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager);

    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType itemType;

    bool controlsCreated;
};

#endif // DBOBJECTVIEWERWIDGET_H
