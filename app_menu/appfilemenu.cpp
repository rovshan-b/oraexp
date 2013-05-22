#include "appfilemenu.h"
#include "util/iconutil.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include "beans/dbitemaction.h"
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
    fileConnectAction=fileMenu->addAction(IconUtil::getIcon("connect"), tr("&Connect"), this, SLOT(showConnectDialog()));
    fileConnectAction->setStatusTip(tr("Connect to database server"));

    toolbar->addAction(fileConnectAction);

    fileMenu->addSeparator();
    toolbar->addSeparator();

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

    fileSaveAction=fileMenu->addAction(IconUtil::getIcon("filesave"), tr("&Save"), this, SLOT(save()), QKeySequence(QKeySequence::Save));
    fileSaveAction->setStatusTip(tr("Save contents of current worksheet"));
    toolbar->addAction(fileSaveAction);
    fileSaveAsAction=fileMenu->addAction(IconUtil::getIcon("filesaveas"), tr("S&ave as..."), this, SLOT(saveAs()), QKeySequence(QKeySequence::SaveAs));
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
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("table"), tr("&Table"), "", "", DbTreeModel::Table, this, SLOT(showCreator()), QKeySequence(tr("Ctrl+T", "File|New table")));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("view"), tr("&View"), "", "", DbTreeModel::View, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("package"), tr("&Package"), "", "", DbTreeModel::Package, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("procedure"), tr("P&rocedure"), "", "", DbTreeModel::Procedure, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("function"), tr("&Function"), "", "", DbTreeModel::Function, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("trigger"), tr("Tri&gger"), "", "", DbTreeModel::Trigger, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("sequence"), tr("&Sequence"), "", "", DbTreeModel::Sequence, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("type"), tr("Typ&e"), "", "", DbTreeModel::Type, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("synonym"), tr("S&ynonym"), "", "", DbTreeModel::Synonym, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("dblink"), tr("Database &link"), "", "", DbTreeModel::DatabaseLink, this, SLOT(showCreator()));
    WidgetHelper::addDbItemAction(fileNewMenu, IconUtil::getIcon("user"), tr("&User"), "", "", DbTreeModel::Schema, this, SLOT(showCreator()));
}

void AppFileMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab)
{
    QList<QAction*> fileNewMenuActions=fileNewMenu->actions();

    fileNewAction->setEnabled(cnPage!=0);
    foreach(QAction *action, fileNewMenuActions){
        action->setEnabled(cnPage!=0);
    }

    fileOpenAction->setEnabled(getConnectionsPane()->currentConnectionPage() != 0);

    fileSaveAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());
    fileSaveAllAction->setEnabled(cnPageTab!=0 && (cnPage->tabCount()>1 || cnPageTab->canSave()));
    fileSaveAsAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());

    filePrintAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());
    filePrintPreviewAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());

}

void AppFileMenu::showConnectDialog()
{
    DialogHelper::showConnectDialog(getConnectionsPane());
}

void AppFileMenu::showCreator()
{
    DbItemAction *action=static_cast<DbItemAction*>(sender());
    Q_ASSERT(action);

    uiManager()->createCreator(action);
}
