#include "appfilemenu.h"
#include "util/iconutil.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include "util/settings.h"
#include "util/settingshelper.h"
#include "util/dbutil.h"
#include "beans/dbitemaction.h"
#include "beans/dbconnectioninfo.h"
#include "beans/environmentinfo.h"
#include <QtGui>

AppFileMenu::AppFileMenu(QMenu *fileMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent)
{
    setupMenu(fileMenu, toolbar);
}

AppFileMenu::~AppFileMenu()
{
    WidgetHelper::deleteActions(connectionListMenu->actions());

    delete connectionListMenu;
    delete reconnectMenu;
    delete fileNewMenu;
    delete recentFilesMenu;
}

void AppFileMenu::setupMenu(QMenu *fileMenu, QToolBar *toolbar)
{
    fileConnectAction=fileMenu->addAction(IconUtil::getIcon("connect"), tr("Connect"), this, SLOT(showConnectDialog()));
    fileConnectAction->setStatusTip(tr("Connect to database server"));

    fileConnectToolbarAction = new QAction(IconUtil::getIcon("connect"), tr("Connect"), this);
    connect(fileConnectToolbarAction, SIGNAL(triggered()), this, SLOT(showConnectDialog()));
    createConnectionListMenu();
    fileConnectToolbarAction->setMenu(connectionListMenu);

    toolbar->addAction(fileConnectToolbarAction);

    QAction *reconnectAction = fileMenu->addAction(tr("Check connections"));
    reconnectMenu = new QMenu();
    reconnectMenu->addAction(IconUtil::getIcon("connect"), tr("All"), this, SLOT(reconnectAll()));
    reconnectMenu->addAction(tr("Current connection"), this, SLOT(reconnectCurrentConnection()));
    reconnectMenu->addAction(tr("Current tab"), this, SLOT(reconnectCurrentTab()));
    reconnectAction->setMenu(reconnectMenu);
    connect(reconnectMenu, SIGNAL(aboutToShow()), this, SLOT(enableReconnectMenuActions()));

    fileMenu->addSeparator();
    toolbar->addSeparator();

    fileNewAction=fileMenu->addAction(IconUtil::getIcon("filenew"), tr("&New"), this, SLOT(addWorksheet()));
    createFileNewMenu();
    fileNewAction->setMenu(fileNewMenu);

    toolbar->addAction(fileNewAction);

    fileOpenAction=fileMenu->addAction(IconUtil::getIcon("fileopen"), tr("&Open..."), this, SLOT(open()), QKeySequence(QKeySequence::Open));
    fileOpenAction->setStatusTip(tr("Open a file in worksheet"));

    fileOpenRecentAction=fileMenu->addAction(tr("Recent files"));
    createRecentFilesMenu();
    fileOpenRecentAction->setMenu(recentFilesMenu);

    fileOpenToolbarAction = new QAction(IconUtil::getIcon("fileopen"), tr("Open..."), this);
    connect(fileOpenToolbarAction, SIGNAL(triggered()), this, SLOT(open()));
    fileOpenToolbarAction->setMenu(recentFilesMenu);

    toolbar->addAction(fileOpenToolbarAction);

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
    fileSaveAllAction=fileMenu->addAction(IconUtil::getIcon("filesaveall"), tr("Sa&ve all"), this, SLOT(saveAll()));
    fileSaveAllAction->setStatusTip(tr("Save contents of all open worksheets"));
    toolbar->addAction(fileSaveAllAction);

    fileMenu->addSeparator();

    filePrintAction=fileMenu->addAction(IconUtil::getIcon("fileprint"), tr("&Print..."));
    filePrintAction->setStatusTip(tr("Print current document"));
    toolbar->addAction(filePrintAction);
    filePrintPreviewAction=fileMenu->addAction(tr("P&rint preview..."));
    filePrintPreviewAction->setStatusTip(tr("Print preview current document"));

    fileMenu->addSeparator();

    QAction *exitAction=fileMenu->addAction(IconUtil::getIcon("exit"), tr("E&xit"), this, SLOT(exitApplication()));
    exitAction->setStatusTip(tr("Quit application"));
}

void AppFileMenu::createConnectionListMenu()
{
    connectionListMenu=new QMenu(tr("Connection list"));

    connect(connectionListMenu, SIGNAL(aboutToShow()), this, SLOT(populateConnectionMenu()));
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

void AppFileMenu::createRecentFilesMenu()
{
    recentFilesMenu = new QMenu(tr("Open recent file..."));

    QStringList list = Settings::value("recentFiles").toStringList();
    for(int i=0; i<qMin(list.size(), 20); ++i){
        recentFilesMenu->addAction(list.at(i), this, SLOT(openRecent()));
    }

    if(list.size() == 0){
        recentFilesMenu->addAction(tr("Empty"))->setEnabled(false);
    }
}

void AppFileMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab)
{
    QList<QAction*> fileNewMenuActions=fileNewMenu->actions();

    fileNewAction->setEnabled(cnPage!=0);
    foreach(QAction *action, fileNewMenuActions){
        action->setEnabled(cnPage!=0);
    }

    fileOpenAction->setEnabled(cnPage != 0);
    fileOpenToolbarAction->setEnabled(cnPage != 0);

    fileOpenRecentAction->setEnabled(cnPage != 0);

    fileSaveAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());
    fileSaveAllAction->setEnabled(cnPageTab!=0 && (cnPage->tabCount()>0 || cnPageTab->canSave()));
    fileSaveAsAction->setEnabled(cnPageTab!=0 && cnPageTab->canSave());

    filePrintAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());
    filePrintPreviewAction->setEnabled(cnPageTab!=0 && cnPageTab->canPrint());

}

void AppFileMenu::addToRecentFileList(const QString &fileName)
{
    QList<QAction*> actions = recentFilesMenu->actions();
    int actionCount = actions.size();
    for(int i=actions.size()-1; i>=0; --i){
        QAction *action = actions.at(i);
        if(action->text()==fileName || action->text()==tr("Empty")){
            recentFilesMenu->removeAction(action);
            action->deleteLater();
            --actionCount;
        }
    }

    QAction *action=new QAction(fileName, this);
    connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));

    if(actionCount > 0){
        recentFilesMenu->insertAction(recentFilesMenu->actions().at(0), action);
    }else{
        recentFilesMenu->addAction(action);
    }
}

void AppFileMenu::saveRecentFileList()
{
    QStringList list;

    QList<QAction*> actions = recentFilesMenu->actions();
    for(int i=0; i<actions.size(); ++i){

        if(actions.at(i)->isEnabled()){
            list.append(actions.at(i)->text());
        }
    }

    Settings::setValue("recentFiles", list);
}

void AppFileMenu::showConnectDialog()
{
    QAction *action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    getConnectionsPane()->addConnection(action->data().toString());
}

void AppFileMenu::reconnectAll()
{
}

void AppFileMenu::reconnectCurrentConnection()
{
}

void AppFileMenu::reconnectCurrentTab()
{
}

void AppFileMenu::enableReconnectMenuActions()
{
    ConnectionsPane *cnPane = getConnectionsPane();
    ConnectionPage *cnPage = cnPane->currentConnectionPage();

    bool hasCurrentConnection = (cnPage != 0 && cnPage->isConnected());

    reconnectMenu->actions().at(1)->setEnabled(hasCurrentConnection);

    if(hasCurrentConnection){
        int cnPageIx = cnPane->indexOf(cnPage);
        reconnectMenu->actions().at(1)->setText(cnPane->tabText(cnPageIx));
        reconnectMenu->actions().at(1)->setIcon(cnPane->tabIcon(cnPageIx));
    }else{
        reconnectMenu->actions().at(1)->setText(tr("Current connection"));
        reconnectMenu->actions().at(1)->setIcon(QIcon());
    }

    ConnectionPageTab *cnPageTab = hasCurrentConnection ? cnPage->currentConnectionPageTab() : 0;
    reconnectMenu->actions().at(2)->setEnabled(cnPageTab != 0);
    if(cnPageTab){
        int cnPageTabIx = cnPage->indexOf(cnPageTab);
        reconnectMenu->actions().at(2)->setText(cnPage->tabText(cnPageTabIx));
        reconnectMenu->actions().at(2)->setIcon(cnPage->tabIcon(cnPageTabIx));
    }else{
        reconnectMenu->actions().at(2)->setText(tr("Current tab"));
        reconnectMenu->actions().at(2)->setIcon(QIcon());
    }

}

void AppFileMenu::showCreator()
{
    DbItemAction *action=static_cast<DbItemAction*>(sender());
    Q_ASSERT(action);

    uiManager()->createCreator(action);
}

void AppFileMenu::openRecent()
{
    QAction *action = static_cast<QAction*>(sender());
    Q_ASSERT(action);
    uiManager()->openRecentFile(action->text());
}

void AppFileMenu::populateConnectionMenu()
{
    WidgetHelper::deleteActions(connectionListMenu->actions());
    connectionListMenu->actions().clear();

    QList<DbConnectionInfo *> allConnections = SettingsHelper::loadConnectionList();

    QList<EnvironmentInfo> environments = DbUtil::getEnvironmentList();
    foreach(const EnvironmentInfo &env, environments){
        QList<DbConnectionInfo *> connections = getConnectionsByEnvironment(allConnections, env.environment);

        QAction *envAction = connectionListMenu->addAction(env.icon, env.title);
        QMenu *cnListMenu = new QMenu();
        envAction->setMenu(cnListMenu);

        foreach(DbConnectionInfo *connection, connections){
            QAction *action = cnListMenu->addAction(env.icon, connection->title, this, SLOT(showConnectDialog()));
            action->setData(connection->uuid);
        }

        if(cnListMenu->actions().size() == 0){
            cnListMenu->addAction(tr("Empty"))->setEnabled(false);
        }
    }

    qDeleteAll(allConnections);
}

void AppFileMenu::exitApplication()
{
    getConnectionsPane()->window()->close();
}

bool titleLessThan(DbConnectionInfo *info1, DbConnectionInfo *info2)
{
    return info1->title.compare(info2->title, Qt::CaseInsensitive) < 0;
}

QList<DbConnectionInfo *> AppFileMenu::getConnectionsByEnvironment(QList<DbConnectionInfo *> allConnections, OraExp::ConnectionEnvironment environment)
{
    QList<DbConnectionInfo *> result;

    for(int i=0; i<allConnections.size(); ++i){
        DbConnectionInfo *connection = allConnections.at(i);
        if(connection->environment == environment){
            result.append(connection);
        }
    }

    qSort(result.begin(), result.end(), titleLessThan);

    return result;
}
