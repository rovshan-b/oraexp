#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectionspane.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "connection_page/connectionpage.h"
#include "app_menu/appmenu.h"
#include <QtGui>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/database.png"));

    connectionsPane=new ConnectionsPane();
    this->setCentralWidget(connectionsPane);

    createMenu();

    DbConnection::initEnvironment();

    setWindowIcon(IconUtil::getIcon("database"));
    setWindowTitle("Oracle Explorer");

    resize(1024, 700);
    //setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{   
    AppMenu::cleanUp();

    delete ui;

    delete connectionsPane; //to ensure that child DbConnection destructors will be called before destroyEnvironment
    DbConnection::destroyEnvironment();
}

void MainWindow::createMenu()
{
    AppMenu::defaultInstance()->setupMenu(this, this->connectionsPane);
}

void MainWindow::showConnectDialog()
{
    DialogHelper::showConnectDialog(this->connectionsPane);
}
