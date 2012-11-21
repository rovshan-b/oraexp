#include "dbobjectviewerwidget.h"
#include "util/iconutil.h"
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

    createMainWidget(layout);

    layout->setContentsMargins(2, 0, 2, 0);

    setLayout(layout);
}

void DbObjectViewerWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler==0);

    this->queryScheduler=queryScheduler;
}

void DbObjectViewerWidget::setObjectName(const QString &schemaName,
                                         const QString &objectName,
                                         DbTreeModel::DbTreeNodeType itemType)
{
    //Q_ASSERT(queryScheduler);

    Q_ASSERT(this->schemaName.isEmpty());

    this->schemaName=schemaName;
    this->objectName=objectName;
    this->itemType=itemType;
}

QList<QAction *> DbObjectViewerWidget::getSpecificToolbarButtons()
{
    return QList<QAction*>();
}
