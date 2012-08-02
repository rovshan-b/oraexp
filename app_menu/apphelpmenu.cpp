#include "apphelpmenu.h"
#include "util/iconutil.h"
#include <QtGui>

AppHelpMenu::AppHelpMenu(QMenu *helpMenu, QToolBar *toolbar, QObject *parent) : QObject(parent)
{
    setupMenu(helpMenu, toolbar);
}

void AppHelpMenu::setupMenu(QMenu *helpMenu, QToolBar *)
{
    helpContentsAction=helpMenu->addAction(IconUtil::getIcon("help"), tr("&Contents"));
    helpContentsAction->setStatusTip(tr("Application documentation"));
}
