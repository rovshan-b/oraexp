#include "codecreator.h"
#include <QtGui>

CodeCreator::CodeCreator(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    creatorWidget = new CodeCreatorWidget(schemaName, objectName, objectType);
}

void CodeCreator::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,2,0,0);

    creatorWidget->createUi();
    connect(creatorWidget, SIGNAL(objectInfoLoaded()), this, SLOT(emitInitCompletedSignal()));

    mainLayout->addWidget(creatorWidget);
    setLayout(mainLayout);

}

void CodeCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    creatorWidget->setQueryScheduler(this);
}
