#include "dbobjectviewertabs.h"
#include "dbobjectviewer.h"
#include "dbobjectviewertoolbar.h"
#include "dbobjectviewergenerictab.h"
#include "context_menu/contextmenuutil.h"
#include "widgets/specbodyswitcherwidget.h"
#include "util/dbutil.h"
#include "util/widgethelper.h"
#include <QtGui>

DbObjectViewerTabs::DbObjectViewerTabs(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager, QWidget *parent) :
    GenericQueryViewerTabs(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType),
    hasSpecBodySwitcher(false),
    objectActionsMenu(0)
{   
}

DbObjectViewerTabs::~DbObjectViewerTabs()
{
    WidgetHelper::deleteMenu(objectActionsMenu);
}


void DbObjectViewerTabs::setHasSpecBodySwitcher(bool hasSpecBodySwitcher)
{
    this->hasSpecBodySwitcher=hasSpecBodySwitcher;
}

void DbObjectViewerTabs::setCurrentTab(int ix)
{
    tabWidget->setCurrentIndex(ix);
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
        objectActionsMenu = new QMenu();
        objectActionsMenu->addActions(actions);

        QToolButton *menuButton = new QToolButton(this);
        menuButton->setText(tr("Object Menu"));
        menuButton->setMenu(objectActionsMenu);
        menuButton->setPopupMode(QToolButton::InstantPopup);
        toolbar->addWidget(menuButton);
        lastSeparatorBeforeProgressBar=toolbar->addSeparator();
    }

    createProgressBarAction();
}
