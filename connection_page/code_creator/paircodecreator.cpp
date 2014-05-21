#include "paircodecreator.h"
#include "util/dbutil.h"
#include <QtGui>


PairCodeCreator::PairCodeCreator(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType objectType,
                                 DbUiManager *uiManager, QWidget *parent) : ConnectionPageTab(uiManager, parent), initializedCount(0)

{
    specCreatorWidget = new CodeCreatorWidget(schemaName, objectName, DbUtil::getSpecType(objectType));
    specCreatorWidget->setHasSpecBodySwitcher(true, true);
    connect(specCreatorWidget, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToBody()));
    connect(specCreatorWidget, SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));
    connect(specCreatorWidget, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

    bodyCreatorWidget = new CodeCreatorWidget(schemaName, objectName, DbUtil::getBodyType(objectType));
    bodyCreatorWidget->setHasSpecBodySwitcher(true, false);
    connect(bodyCreatorWidget, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToSpec()));
    connect(bodyCreatorWidget, SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));
    connect(bodyCreatorWidget, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));

    isSpecType = DbUtil::isSpecType(objectType);
}

void PairCodeCreator::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,2,0,0);

    specCreatorWidget->createUi();
    bodyCreatorWidget->createUi();

    connect(specCreatorWidget, SIGNAL(objectInfoLoaded()), this, SLOT(creatorInitialized()));
    connect(bodyCreatorWidget, SIGNAL(objectInfoLoaded()), this, SLOT(creatorInitialized()));

    tab=new QStackedWidget();
    tab->setContentsMargins(0,0,0,0);
    tab->addWidget(specCreatorWidget);
    tab->addWidget(bodyCreatorWidget);
    tab->setCurrentIndex(isSpecType ? 0 : 1);

    mainLayout->addWidget(tab);
    setLayout(mainLayout);
}

void PairCodeCreator::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    specCreatorWidget->setQueryScheduler(this);
    bodyCreatorWidget->setQueryScheduler(this);
}

void PairCodeCreator::focusAvailable()
{
    currentCreator()->focusAvailable();
}

bool PairCodeCreator::isModified(int childIndex) const
{
    Q_UNUSED(childIndex);
    return specCreatorWidget->isModified() || bodyCreatorWidget->isModified();
}

void PairCodeCreator::setModified(bool modified, int childIndex)
{
    childCreator(childIndex)->setModified(modified);
}

QString PairCodeCreator::getDefaultSaveSuffix() const
{
    return currentCreator()->getDefaultSaveSuffix();
}

QList<QTextDocument *> PairCodeCreator::getSavePreviewDocuments() const
{
    QList<QTextDocument *> result;

    if(specCreatorWidget->isModified()){
        result.append(specCreatorWidget->getTextDocument());
    }

    if(bodyCreatorWidget->isModified()){
        result.append(bodyCreatorWidget->getTextDocument());
    }

    return result;
}

bool PairCodeCreator::isSaved() const
{
    return !specCreatorWidget->getCurrentFileName().isEmpty() &&
           !bodyCreatorWidget->getCurrentFileName().isEmpty();
}

QString PairCodeCreator::getCurrentFileName(int childIndex) const
{
    return childCreator(childIndex)->getCurrentFileName();
}

void PairCodeCreator::setCurrentFileName(const QString &fileName, int childIndex)
{
    childCreator(childIndex)->setCurrentFileName(fileName);
}

QList<OraExp::SidePane> PairCodeCreator::getRequestedSidePanes() const
{
    return (QList<OraExp::SidePane>() << OraExp::SidePaneCodeStructure);
}

void PairCodeCreator::saveToStream(QTextStream &out, int childIndex)
{
    out << childCreator(childIndex)->getContents();
}

void PairCodeCreator::creatorInitialized()
{
    if(++initializedCount==2){
        emitInitCompletedSignal();
    }
}

void PairCodeCreator::switchToSpec()
{
    tab->setCurrentIndex(0);
}

void PairCodeCreator::switchToBody()
{
    tab->setCurrentIndex(1);
}

void PairCodeCreator::modificationChanged(bool)
{
    setModifiedStatusToCaption(isModified());
}

bool PairCodeCreator::saveAll()
{
    if(specCreatorWidget->isModified()){
        if(!saveContents(1)){
            return false;
        }
    }

    if(bodyCreatorWidget->isModified()){
        if(!saveContents(2)){
            return false;
        }
    }

    return true;
}

CodeCreatorWidget *PairCodeCreator::currentCreator() const
{
    return tab->currentIndex()==0 ? specCreatorWidget : bodyCreatorWidget;
}

CodeCreatorWidget *PairCodeCreator::childCreator(int childIndex) const
{
    if(childIndex == 0){
        return currentCreator();
    }else if(childIndex == 1){
        return specCreatorWidget;
    }else{
        return bodyCreatorWidget;
    }
}
