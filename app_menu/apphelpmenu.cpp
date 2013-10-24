#include "apphelpmenu.h"
#include "util/iconutil.h"
#include "mainwindow.h"
#include "dialogs/aboutdialog.h"
#include <QtGui>

AppHelpMenu::AppHelpMenu(QMenu *helpMenu, QToolBar *toolbar, QObject *parent) : QObject(parent)
{
    setupMenu(helpMenu, toolbar);
}

void AppHelpMenu::setupMenu(QMenu *helpMenu, QToolBar *)
{
    helpContentsAction=helpMenu->addAction(IconUtil::getIcon("help"), tr("&Contents"), this, SLOT(displayHelpContents()));
    helpContentsAction->setStatusTip(tr("Application documentation"));

    helpMenu->addSeparator();

    helpAboutAction=helpMenu->addAction(IconUtil::getIcon("about"), tr("&About"), this, SLOT(displayAboutBox()));
    helpAboutAction->setStatusTip(tr("About application"));
}

void AppHelpMenu::displayHelpContents()
{
    if(QMessageBox::question(MainWindow::defaultInstance(), tr("Open URL"),
                             tr("Your defaut web bowser will be opened and navigated to http://www.oraexp.com/docs. Do you want to continue?"),
                             QMessageBox::Ok|QMessageBox::Cancel)!=QMessageBox::Ok){
        return;
    }

    QDesktopServices::openUrl(QUrl("http://www.oraexp.com/docs"));
}

void AppHelpMenu::displayAboutBox()
{
    AboutDialog dialog(MainWindow::defaultInstance());
    dialog.exec();
}
