#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "connectioneditordialog.h"
#include "connectivity/dbconnection.h"
#include "mytablewidgetitem.h"
#include "util/iconutil.h"
#include "passwordentrydialog.h"
#include "ui_passwordentrydialog.h"
#include "util/strutil.h"
#include <QtGui>
#include "connectivity/ociexception.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    currentDb(0)
{
    ui->setupUi(this);

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    loadConnectionsFromConfig();
    if(ui->tableWidget->rowCount()>0){
        ui->tableWidget->selectRow(0);
    }

    connect(ui->tableWidget, SIGNAL(activated(QModelIndex)), this, SLOT(tableActivated()));

    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addConnection()));
    connect(ui->actionEdit, SIGNAL(triggered()), this, SLOT(editConnection()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteConnection()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectToServer()));

    ui->tableWidget->addAction(ui->actionConnect);
    ui->tableWidget->addAction(ui->actionAdd);
    ui->tableWidget->addAction(ui->actionEdit);
    ui->tableWidget->addAction(ui->actionDelete);
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->tableWidget->setFocus();

    QSettings settings;
    restoreGeometry(settings.value("connectDialogGeometry").toByteArray());

    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(width()/3-20);

    //QTimer::singleShot(0, this, SLOT(centerWindow()));
    //centerWindow();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

//void ConnectDialog::centerWindow()
//{
//    QRect frect = frameGeometry();
//    QDesktopWidget *desktop=qApp->desktop();
//    frect.moveCenter(desktop->availableGeometry(desktop->screenNumber(this->parentWidget())).center());
//    move(frect.topLeft());
//}

void ConnectDialog::showConnectionEditorDialog(DbConnection *db, int currentRow)
{
    ConnectionEditorDialog editorDialog(this);
    if(db!=NULL){
        editorDialog.setDbConnection(db);
    }
    if(editorDialog.exec())
    {
        DbConnection *db=editorDialog.getDbConnection();
        disableSortingAndSaveConnectionInfo(db, currentRow);
    }
}

void ConnectDialog::on_connectButton_clicked()
{
    connectToServer();
}

void ConnectDialog::on_addConnectionButton_clicked()
{
    addConnection();
}

void ConnectDialog::addConnection()
{
    showConnectionEditorDialog(NULL, -1);
}

void ConnectDialog::on_editConnectionButton_clicked()
{
    editConnection();
}

void ConnectDialog::editConnection()
{
    int currentRow=ui->tableWidget->currentRow();
    if(currentRow==-1){
        QMessageBox::warning(this, tr("No item selected"), tr("Please, select an item from list to modify its properties"));
        return;
    }

    MyTableWidgetItem *name=(MyTableWidgetItem*)ui->tableWidget->item(currentRow, 0);
    DbConnection *db=name->getDbConnection();
    showConnectionEditorDialog(db, currentRow);
}

void ConnectDialog::on_deleteConnectionButton_clicked()
{
    deleteConnection();
}

void ConnectDialog::deleteConnection()
{
    int currentRow=ui->tableWidget->currentRow();
    if(currentRow==-1){
        QMessageBox::warning(this, tr("No item selected"), tr("Please, select an item from list to delete"));
        return;
    }

    QMessageBox::StandardButton confirmation=(QMessageBox::StandardButton)QMessageBox::question(this, tr("Confirm deletion"),
                                                                                                tr("Do you really want to delete selected connection?"),
                                                                                                QMessageBox::Yes, QMessageBox::No);
    if(confirmation==QMessageBox::Yes){
        ui->tableWidget->removeRow(currentRow);
    }
}

void ConnectDialog::disableSortingAndSaveConnectionInfo(DbConnection *db, int currentRow)
{
     ui->tableWidget->setSortingEnabled(false);
     saveConnectionInfo(db, currentRow);
     ui->tableWidget->setSortingEnabled(true);
}

void ConnectDialog::saveConnectionInfo(DbConnection *db, int currentRow)
{
    if(currentRow==-1){
        int rowCount=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        currentRow=rowCount;
    }


    MyTableWidgetItem *name=new MyTableWidgetItem(db->getTitle(), db); //db will be deleted in destructor of MyTableWidgetItem
    name->setIcon(IconUtil::getEnvironmentIcon(db->getEnvironment()));
    QTableWidgetItem *username=new QTableWidgetItem(db->getUsername());
    QTableWidgetItem *server=new QTableWidgetItem(db->getTnsName());

    ui->tableWidget->setItem(currentRow, 0, name);
    ui->tableWidget->setItem(currentRow, 1, username);
    ui->tableWidget->setItem(currentRow, 2, server);
}

void ConnectDialog::saveConnectionsToConfig()
{
    int rowCount=ui->tableWidget->rowCount();

    QSettings settings;

    MyTableWidgetItem *name=NULL;
    DbConnection *db=NULL;

    settings.remove("connections");
    settings.beginWriteArray("connections");
    for(int counter=0; counter<rowCount; ++counter){
        name=(MyTableWidgetItem*)ui->tableWidget->item(counter, 0);
        db=name->getDbConnection();

        settings.setArrayIndex(counter);
        settings.setValue("db", qVariantFromValue(*db));
    }
    settings.endArray();
}

void ConnectDialog::loadConnectionsFromConfig()
{
    ui->tableWidget->setSortingEnabled(false);

    QSettings settings;

    int size = settings.beginReadArray("connections");
    for (int counter = 0; counter < size; ++counter) {
         settings.setArrayIndex(counter);
         QVariant val=settings.value("db");
         if(val.isValid()){
             DbConnection *db=new DbConnection(val.value<DbConnection>());
             saveConnectionInfo(db, -1);
         }
     }
    settings.endArray();

    ui->tableWidget->setSortingEnabled(true);
}

void ConnectDialog::tableActivated()
{
    connectToServer();
}

void ConnectDialog::connectToServer()
{
    int currentRow=ui->tableWidget->currentRow();
    if(currentRow==-1){
        QMessageBox::warning(this, tr("No item selected"), tr("Please, select an item from list to connect to"));
        return;
    }

    MyTableWidgetItem *name=(MyTableWidgetItem*)ui->tableWidget->item(currentRow, 0);
    DbConnection *db=name->getDbConnection();

    if(!db->getSavePassword() && db->getUsername().length()>0 && !promptForPassword(db)){
        return;
    }

    tryToEstablishConnection(db);
}

bool ConnectDialog::promptForPassword(DbConnection *db)
{
    PasswordEntryDialog pwdDialog(this);
    if(pwdDialog.exec()){
        db->setPassword(pwdDialog.ui->passwordLineEdit->text());
        db->setSavePassword(pwdDialog.ui->savePasswordCheckBox->isChecked());

        return true;
    }else{
        return false;
    }
}

void ConnectDialog::tryToEstablishConnection(DbConnection *db)
{
    currentDb=new DbConnection(*db);

    try{
        currentDb->connect();
        db->setOciOwner(false); //currentDb takes responsibility to free oci environment

        QSettings settings;
        settings.setValue("connectDialogGeometry", saveGeometry());

        accept();
    }catch(OciException &ex){
        delete currentDb;
        currentDb=0;

        QMessageBox::critical(this, tr("Connection failed"), ex.getErrorMessage());
    }
}

DbConnection *ConnectDialog::getConnection() const
{
    return currentDb;
}
