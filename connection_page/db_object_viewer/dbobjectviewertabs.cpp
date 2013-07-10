#include "dbobjectviewertabs.h"
#include "dbobjectviewer.h"
#include "dbobjectviewertoolbar.h"
#include "dbobjectviewergenerictab.h"
#include "context_menu/contextmenuutil.h"
#include "widgets/specbodyswitcherwidget.h"
#include "util/dbutil.h"
#include <QtGui>

DbObjectViewerTabs::DbObjectViewerTabs(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager, QWidget *parent) :
    GenericQueryViewerTabs(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType),
    hasSpecBodySwitcher(false)
{   
}


void DbObjectViewerTabs::setHasSpecBodySwitcher(bool hasSpecBodySwitcher)
{
    this->hasSpecBodySwitcher=hasSpecBodySwitcher;
}

void DbObjectViewerTabs::initTab(GenericQueryViewerWidget *tab)
{
    static_cast<DbObjectViewerGenericTab*>(tab)->setObjectName(schemaName, objectName, itemType);

    GenericQueryViewerTabs::initTab(tab);
}

void DbObjectViewerTabs::createToolbarButtons()
{
    createRefreshButton();

    if(hasSpecBodySwitcher){
        toolbar->addSeparator();
        SpecBodySwitcherWidget *switcher=new SpecBodySwitcherWidget(DbUtil::isSpecType(this->itemType), this);
        toolbar->addWidget(switcher);
        connect(switcher, SIGNAL(specBodySwitchRequested()), this, SIGNAL(specBodySwitchRequested()));
    }

    lastSeparatorBeforeProgressBar=toolbar->addSeparator();

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(
                               this->schemaName, this->objectName, "",
                               itemType, uiManager);

    if(actions.size()>0){
        foreach(QAction *action, actions){

            if(action->menu()){ //convert action to QToolButton
                QMenu *actionMenu = action->menu();
                QToolButton *button = new QToolButton(toolbar);
                button->setText(action->text());
                button->setIcon(action->icon());
                action->setMenu(0);
                button->setMenu(actionMenu);
                button->setPopupMode(QToolButton::InstantPopup);
                actionMenu->setParent(this, Qt::Popup);
                toolbar->addWidget(button);

                delete action;
            }else{
                toolbar->addAction(action);
                action->setParent(toolbar);
            }
        }
        lastSeparatorBeforeProgressBar=toolbar->addSeparator();
    }

    createProgressBarAction();
}
