#include "appfilemenu.h"
#include "util/iconutil.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include <QtGui>

AppFileMenu::AppFileMenu(QMenu *fileMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent)
{
    setupMenu(fileMenu, toolbar);
}

AppFileMenu::~AppFileMenu()
{
    delete fileNewMenu;
}

void AppFileMenu::setupMenu(QMenu *fileMenu, QToolBar *toolbar)
{
    fileNewAction=fileMenu->addAction(IconUtil::getIcon("filenew"), tr("&New"), this, SLOT(addWorksheet()));
    createFileNewMenu();
    fileNewAction->setMenu(fileNewMenu);

    toolbar->addAction(fileNewAction);

    fileOpenAction=fileMenu->addAction(IconUtil::getIcon("fileopen"), tr("&Open..."), this, SLOT(open()), QKeySequence(QKeySequence::Open));
    fileOpenAction->setStatusTip(tr("Open a file in worksheet"));
    toolbar->addAction(fileOpenAction);

    fileMenu->addSeparator();

    fileCloseAction=fileMenu->addAction(IconUtil::getIcon("fileclose"), tr("&Close"));
    fileCloseAction->setStatusTip(tr("Close active tab"));
    fileCloseAllAction=fileMenu->addAction(tr("Close &All"));
    fileCloseAllAction->setStatusTip(tr("Close all tabs"));

    fileMenu->addSeparator();

    fileSaveAction=fileMenu->addAction(IconUtil::getIcon("filesave"), tr("&Save"));
    fileSaveAction->setStatusTip(tr("Save contents of current worksheet"));
    toolbar->addAction(fileSaveAction);
    fileSaveAsAction=fileMenu->addAction(IconUtil::getIcon("filesaveas"), tr("S&ave as..."));
    fileSaveAsAction->setStatusTip(tr("Save contents of current worksheet to specified file"));
    fileSaveAllAction=fileMenu->addAction(IconUtil::getIcon("filesaveall"), tr("Sa&ve all"));
    fileSaveAllAction->setStatusTip(tr("Save contents of all open worksheets"));
    toolbar->addAction(fileSaveAllAction);

    fileMenu->addSeparator();

    filePrintAction=fileMenu->addAction(IconUtil::getIcon("fileprint"), tr("&Print..."));
    filePrintAction->setStatusTip(tr("Print current document"));
    toolbar->addAction(filePrintAction);
    filePrintPreviewAction=fileMenu->addAction(tr("P&rint preview..."));
    filePrintPreviewAction->setStatusTip(tr("Print preview current document"));

    fileMenu->addSeparator();

    QAction *exitAction=fileMenu->addAction(IconUtil::getIcon("exit"), tr("E&xit"));
    exitAction->setStatusTip(tr("Quit application"));
}

void AppFileMenu::createFileNewMenu()
{
    fileNewMenu=new QMenu(tr("Create new..."));

    fileNewMenu->addAction(IconUtil::getIcon("worksheet"), tr("&Worksheet"), this, SLOT(addWorksheet()), QKeySequence(tr("Ctrl+N", "File|New worksheet")));
    fileNewMenu->addAction(IconUtil::getIcon("table"), tr("&Table"), this, SLOT(showTableCreator()), QKeySequence(tr("Ctrl+T", "File|New table")));
}

void AppFileMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab)
{
    QList<QAction*> fileNewMenuActions=fileNewMenu->actions();

    fileNewAction->setEnabled(cnPage!=0);
    foreach(QAction *action, fileNewMenuActions){
        action->setEnabled(cnPage!=0);
    }

    fileOpenAction->setEnabled(cnPageTab!=0 && cnPageTab->canOpen());

    fileSaveAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());
    fileSaveAllAction->setEnabled(cnPageTab!=0 && (cnPage->tabCount()>1 || cnPageTab->canSave()));
    fileSaveAsAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());

    filePrintAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());
    filePrintPreviewAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());

}
