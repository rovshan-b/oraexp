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
    connect(creatorWidget, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

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

bool CodeCreator::isModified(int childIndex) const
{
    Q_UNUSED(childIndex);
    return creatorWidget->isModified();
}

void CodeCreator::setModified(bool modified, int childIndex)
{
    Q_UNUSED(childIndex);
    creatorWidget->setModified(modified);
}

QString CodeCreator::getDefaultSaveSuffix() const
{
    return creatorWidget->getDefaultSaveSuffix();
}

QList<QTextDocument *> CodeCreator::getSavePreviewDocuments() const
{
    QList<QTextDocument *> result;

    if(isModified()){
        result.append(creatorWidget->getTextDocument());
    }

    return result;
}

QString CodeCreator::getCurrentFileName(int childIndex) const
{
    Q_UNUSED(childIndex);
    return creatorWidget->getCurrentFileName();
}

void CodeCreator::setCurrentFileName(const QString &fileName, int childIndex)
{
    Q_UNUSED(childIndex);
    creatorWidget->setCurrentFileName(fileName);
}

void CodeCreator::saveToStream(QTextStream &out, int childIndex)
{
    Q_UNUSED(childIndex);
    out << creatorWidget->getContents();
}

void CodeCreator::modificationChanged(bool changed)
{
    setModifiedStatusToCaption(changed);
}
