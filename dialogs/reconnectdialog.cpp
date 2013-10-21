#include "reconnectdialog.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpageobject.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

ReconnectDialog::ReconnectDialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(tr("Checking connections..."));

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

    table->horizontalHeader()->setDefaultSectionSize((((float)this->width())/model->columnCount())-10);
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
        }else{
            ownerItem->setText(tr("Cached connection"));
        }

        QStandardItem *statusItem = new QStandardItem(tr("Checking..."));

        model->appendRow(QList<QStandardItem*>() << captionItem << ownerItem << statusItem);
    }
}

void ReconnectDialog::startChecking()
{
}
