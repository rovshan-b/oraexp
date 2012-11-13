#include "dbobjectviewerwidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "context_menu/contextmenuutil.h"
#include <QtGui>

DbObjectViewerWidget::DbObjectViewerWidget(QWidget *parent) :
        OnDemandInfoViewerWidget(parent),
        queryScheduler(0),
        controlsCreated(false)
{

}

void DbObjectViewerWidget::createChildControls(DbTreeModel::DbTreeNodeType itemType,
                                               DbUiManager *uiManager)
{
    Q_ASSERT(!controlsCreated);

    this->controlsCreated=true;
    QVBoxLayout *layout=new QVBoxLayout();
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16,16));
    createToolbarButtons(itemType, uiManager);
    layout->addWidget(toolbar);

    createMainWidget(layout);

    layout->setContentsMargins(2, 0, 2, 0);

    setLayout(layout);
}

void DbObjectViewerWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler==0);

    this->queryScheduler=queryScheduler;
}

void DbObjectViewerWidget::setObjectName(const QString &schemaName, const QString &tableName)
{
    //Q_ASSERT(queryScheduler);

    Q_ASSERT(this->schemaName.isEmpty());

    this->schemaName=schemaName;
    this->objectName=tableName;
}

void DbObjectViewerWidget::createToolbarButtons(DbTreeModel::DbTreeNodeType itemType,
                                                DbUiManager *uiManager)
{
    refreshButton=new QAction(IconUtil::getIcon("refresh"), tr("Refresh"), this);
    connect(refreshButton, SIGNAL(triggered()), this, SLOT(refreshInfo()));
    toolbar->addAction(refreshButton);

    addSpecificToolbarButtons();

    toolbar->addSeparator();

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(this->schemaName, this->objectName,
                                         itemType, uiManager);

    if(actions.size()>0){
        foreach(QAction *action, actions){
            toolbar->addAction(action);
            action->setParent(toolbar);
        }
        toolbar->addSeparator();
    }

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}

void DbObjectViewerWidget::addSpecificToolbarButtons()
{

}
