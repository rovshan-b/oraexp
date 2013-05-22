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
    connect(creatorWidget, SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));

    mainLayout->addWidget(creatorWidget);
    setLayout(mainLayout);

}

void CodeCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    creatorWidget->setQueryScheduler(this);
}

void CodeCreator::focusAvailable()
{
    creatorWidget->focusAvailable();
}

bool CodeCreator::isModified() const
{
    return creatorWidget->isModified();
}

void CodeCreator::setModified(bool modified)
{
    creatorWidget->setModified(modified);
}

QString CodeCreator::getCurrentFileName() const
{
    return creatorWidget->getCurrentFileName();
}

void CodeCreator::setCurrentFileName(const QString &fileName)
{
    creatorWidget->setCurrentFileName(fileName);
}

void CodeCreator::saveToStream(QTextStream &out)
{
    out << creatorWidget->getContents();
}

void CodeCreator::modificationChanged(bool changed)
{
    setModifiedStatusToCaption(changed);
}
