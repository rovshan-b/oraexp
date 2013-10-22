#include "reconnectdialog.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "util/asyncreconnect.h"
#include "util/dialoghelper.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpageobject.h"
#include "connectivity/dbconnection.h"
#include "mainwindow.h"
#include "connectionspane.h"
#include <QtGui>

ReconnectDialog::ReconnectDialog(QWidget *parent) :
    QDialog(parent), activeThreadCount(0)
{
    setAttribute(Qt::WA_DeleteOnClose);

    DialogHelper::showMaximizeMinimizeButtons(this);

    setWindowTitle(tr("Reconnect"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    table = new DataTable();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSortingEnabled(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    model = new QStandardItemModel(0, 3, this);
    model->setHorizontalHeaderLabels(QStringList() << tr("Connection") << tr("Owner") << tr("Status"));
    table->setModel(model);
    table->horizontalHeader()->setStretchLastSection(true);

    mainLayout->addWidget(table);

    setLayout(mainLayout);

    QSize hint = sizeHint();
    resize(qMax(500, hint.width()), qMax(300, hint.height()));

    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(itemActivated(QModelIndex)));
}

ReconnectDialog::~ReconnectDialog()
{
    qDeleteAll(connections);
}

void ReconnectDialog::setConnections(const QList< Triple<ConnectionPage *, ConnectionPageObject *, DbConnection*> * > &connections)
{
    Q_ASSERT(this->connections.isEmpty());

    this->connections = connections;

    for(int i=0; i<connections.size(); ++i){
        Triple<ConnectionPage *, ConnectionPageObject *, DbConnection*> *triple = connections.at(i);
        QStandardItem *captionItem = new QStandardItem(IconUtil::getEnvironmentIcon(triple->third->getEnvironment()), triple->third->getTitle());
        QStandardItem *ownerItem = new QStandardItem();
        if(triple->second){
            ownerItem->setText(triple->second->getDisplayName());
            ownerItem->setIcon(triple->second->getIcon());

            triple->second->increaseRefCount();
        }else{
            ownerItem->setText(tr("Cached connection"));
        }

        QStandardItem *statusItem = new QStandardItem(tr("Reconnecting..."));

        model->appendRow(QList<QStandardItem*>() << captionItem << ownerItem << statusItem);

        AsyncReconnect *asyncReconnect = new AsyncReconnect(triple->third, this);
        connect(asyncReconnect, SIGNAL(reconnected(DbConnection*,bool,OciException)), this, SLOT(reconnected(DbConnection*,bool,OciException)));
        ++activeThreadCount;
        asyncReconnect->start();
    }
}

void ReconnectDialog::reject()
{
    setResult(QDialog::Rejected);

    if(canClose()){
        QDialog::reject();
    }
}

void ReconnectDialog::closeEvent(QCloseEvent *e)
{
    e->setAccepted(canClose());
}

bool ReconnectDialog::canClose()
{
    if(activeThreadCount > 0){
        QMessageBox::information(this, tr("Dialog busy"),
                                 tr("Cannot close when reconnecting is in progress"));
        return false;
    }

    return true;
}

void ReconnectDialog::resizeEvent(QResizeEvent *)
{
    table->horizontalHeader()->setDefaultSectionSize((((float)this->width())/model->columnCount())-10);
}

void ReconnectDialog::reconnected(DbConnection *db, bool error, const OciException &ex)
{
    int ix = indexOf(db);
    Q_ASSERT(ix != -1);

    QStandardItem *statusItem = model->item(ix, 2);

    if(error){
        statusItem->setText(tr("Error: %1").arg(ex.getErrorMessage()));
    }else{
        statusItem->setText(tr("Success"));
    }

    ConnectionPageObject *obj = connections.at(ix)->second;
    if(obj){
        obj->decreaseRefCount();
    }

    --activeThreadCount;
}

void ReconnectDialog::itemActivated(const QModelIndex &index)
{
    int rowIx = index.row();

    if(rowIx < 0 || rowIx >= connections.size()){
        return;
    }

    Triple<ConnectionPage *, ConnectionPageObject *, DbConnection*> *triple = connections.at(rowIx);
    if(!triple->second){
        return;
    }

    ConnectionPage *cnPage = triple->first;
    ConnectionPageObject *obj = triple->second;

    MainWindow::defaultInstance()->getConnectionsPane()->activateChildWidget(cnPage, obj);
}

int ReconnectDialog::indexOf(DbConnection *db)
{
    for(int i=0; i<connections.size(); ++i){
        if(connections.at(i)->third == db){
            return i;
        }
    }

    return -1;
}
