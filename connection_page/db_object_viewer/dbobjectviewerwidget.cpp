#include "dbobjectviewerwidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include <QtGui>

DbObjectViewerWidget::DbObjectViewerWidget(QWidget *parent) :
        OnDemandInfoViewerWidget(parent),
        queryScheduler(0),
        controlsCreated(false)
{

}

void DbObjectViewerWidget::createChildControls()
{
    Q_ASSERT(!controlsCreated);

    this->controlsCreated=true;
    QVBoxLayout *layout=new QVBoxLayout();
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16,16));
    createToolbarButtons();
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
    Q_ASSERT(queryScheduler);

    Q_ASSERT(this->schemaName.isEmpty());

    this->schemaName=schemaName;
    this->tableName=tableName;
}

void DbObjectViewerWidget::createToolbarButtons()
{
    refreshButton=new QAction(IconUtil::getIcon("refresh"), tr("Refresh"), this);
    connect(refreshButton, SIGNAL(triggered()), this, SLOT(refreshInfo()));
    toolbar->addAction(refreshButton);

    addSpecificToolbarButtons();

    toolbar->addSeparator();

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}

void DbObjectViewerWidget::addSpecificToolbarButtons()
{

}
