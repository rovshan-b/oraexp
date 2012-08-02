#include "tableinfoviewerwidget.h"
#include "tableinfotoolbar.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include <QtGui>

TableInfoViewerWidget::TableInfoViewerWidget(QWidget *parent) :
        OnDemandInfoViewerWidget(parent),
        queryScheduler(0),
        controlsCreated(false)
{

}

void TableInfoViewerWidget::createChildControls()
{
    Q_ASSERT(!controlsCreated);

    this->controlsCreated=true;
    QVBoxLayout *layout=new QVBoxLayout();
    toolbar=new TableInfoToolbar();
    createToolbarButtons();
    layout->addWidget(toolbar);

    createMainWidget(layout);

    layout->setContentsMargins(2, 0, 2, 0);

    setLayout(layout);
}

void TableInfoViewerWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    if(this->queryScheduler==0){
        this->queryScheduler=queryScheduler;
    }
}

void TableInfoViewerWidget::setTableName(const QString &schemaName, const QString &tableName)
{
    Q_ASSERT(queryScheduler);

    Q_ASSERT(this->schemaName.isEmpty());
    //if(this->schemaName.isEmpty()){
        this->schemaName=schemaName;
        this->tableName=tableName;
    //}
}

void TableInfoViewerWidget::createMainWidget(QLayout *)
{

}

void TableInfoViewerWidget::createToolbarButtons()
{
    refreshButton=new QAction(IconUtil::getIcon("refresh"), tr("Refresh"), this);
    connect(refreshButton, SIGNAL(triggered()), this, SLOT(refreshInfo()));
    toolbar->addAction(refreshButton);

    addSpecificToolbarButtons();

    toolbar->addSeparator();

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}

void TableInfoViewerWidget::addSpecificToolbarButtons()
{

}
