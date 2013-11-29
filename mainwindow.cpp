#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectionspane.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "util/settingshelper.h"
#include "util/appconnectionmanager.h"
#include "connection_page/connectionpage.h"
#include "app_menu/appmenu.h"
#include "app_menu/appfilemenu.h"
#include <QtGui>
#include <iostream>

using namespace std;

MainWindow *MainWindow::instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/database.png"));

    connectionsPane=new ConnectionsPane();
    this->setCentralWidget(connectionsPane);

    createMenu();

    setWindowIcon(IconUtil::getIcon("database"));
    setWindowTitle("OraExp");

    SettingsHelper::restoreWindowPosition(this, "MainWindow");
    SettingsHelper::loadStaticApplicationSettings();

    //connectionsPane->installEventFilter(this);

    connect(connectionsPane, SIGNAL(canExit()), this, SLOT(canExit()));

    Q_ASSERT(MainWindow::instance == 0);
    MainWindow::instance = this;

    showConnectDialog();
}

MainWindow::~MainWindow()
{   
    AppMenu::cleanUp();

    delete ui;

    delete connectionsPane; //to ensure that child DbConnection destructors will be called before destroyEnvironment
    DbConnection::destroyEnvironment();

    AppConnectionManager::cleanup();
}

void MainWindow::createMenu()
{
    AppMenu::defaultInstance()->setupMenu(this, this->connectionsPane);
}

void MainWindow::showConnectDialog()
{
    this->connectionsPane->addConnection();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->setAccepted(false);

    if(AppMenu::isAllDisabled()){ //exiting
        return;
    }

    if(connectionsPane->isBusy()){
        QMessageBox::information(this->window(), tr("Application busy"),
                                 tr("Cannot exit when there are ongoing database operations."));
        return;
    }

    bool exiting = connectionsPane->closeAll(true);

    if(exiting){
        AppMenu::disableAll();
    }
}

void MainWindow::canExit()
{
    SettingsHelper::saveWindowPosition(this, "MainWindow");
    SettingsHelper::saveStaticApplicationSettings();
    AppMenu::defaultInstance()->getFileMenu()->saveRecentFileList();

    qApp->quit();
}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj==connectionsPane && ev->type()==QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
        if(keyEvent->key()==Qt::Key_Tab && (keyEvent->modifiers() & Qt::ControlModifier)==Qt::ControlModifier){
            qDebug("Ctrl+Tab pressed");
            ev->setAccepted(true);
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, ev);
}*/
