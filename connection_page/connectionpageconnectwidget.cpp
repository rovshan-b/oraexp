#include "connectionpageconnectwidget.h"
#include "widgets/datatable.h"
#include "widgets/connectioneditor.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/asyncconnect.h"
#include "util/settings.h"
#include "util/settingshelper.h"
#include "models/connectionlistmodel.h"
#include "beans/dbconnectioninfo.h"
#include "connectivity/dbconnection.h"
#include "widgets/lineeditwithclearbutton.h"
#include "passwordentrydialog.h"
#include "models/sortfilterproxymodel.h"
#include <QtGui>

QWeakPointer<ConnectionListModel> ConnectionPageConnectWidget::globalModel;

ConnectionPageConnectWidget::ConnectionPageConnectWidget(const QString &connectionUuid,
                                                         QWidget *parent) :
    QWidget(parent), initialConnectionUuid(connectionUuid), modifyingConnection(false), busy(false)
{
    createUi();
}

void ConnectionPageConnectWidget::createUi()
{
    QHBoxLayout *mainLayout = new QHBoxLayout();

    QSplitter *splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);

    createConnectionList(splitter);
    createConnectionEditor(splitter);

    mainLayout->addWidget(splitter);
    splitter->setSizes(QList<int>() << 600 << 300);
    setLayout(mainLayout);

    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentConnectionChanged(QModelIndex,QModelIndex)));
    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(connectionActivated(QModelIndex)));

    if(!initialConnectionUuid.isEmpty()){
        QTimer::singleShot(0, this, SLOT(connectToInitialUuid()));
    }
}

void ConnectionPageConnectWidget::createConnectionList(QSplitter *splitter)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *w = new QWidget();
    layout->setContentsMargins(0,0,2,0);
    w->setLayout(layout);

    filterEditor = new LineEditWithClearButton();
    filterEditor->lineEdit()->setPlaceholderText(tr("filter..."));
    layout->addWidget(filterEditor);

    table = new DataTable();
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setDefaultSectionSize(250);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    if(ConnectionPageConnectWidget::globalModel.isNull()){ //models can be deleted only on primary thread. so we can be confident that result of isNull will not be changed between calls
        model = QSharedPointer<ConnectionListModel>(new ConnectionListModel, &QObject::deleteLater);
        ConnectionPageConnectWidget::globalModel = model;
        loadConnectionList();
    }else{
        model = ConnectionPageConnectWidget::globalModel.toStrongRef();
    }

    proxyModel = new SortFilterProxyModel(this);
    proxyModel->setSourceModel(model.data());
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    int sortCol = Settings::value("ConnectionPage/SortIndex", -1).toInt();
    int sortOrder = Settings::value("ConnectionPage/SortOrder", 1).toInt();

    table->setModel(proxyModel);
    table->setSortingEnabled(true);
    table->sortByColumn(sortCol, sortOrder ? Qt::AscendingOrder : Qt::DescendingOrder);

    layout->addWidget(table);

    splitter->addWidget(w);

    connect(model.data(), SIGNAL(connectionModified(DbConnectionInfo*)), this, SLOT(connectionModified(DbConnectionInfo*)));
    connect(filterEditor->lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(filterList(QString)));

    filterEditor->lineEdit()->installEventFilter(this);
}

void ConnectionPageConnectWidget::createConnectionEditor(QSplitter *splitter)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(2,0,0,0);
    QWidget *w = new QWidget();
    w->setLayout(layout);

    connectionEditor = new ConnectionEditor();
    layout->addWidget(WidgetHelper::createGroupBox(connectionEditor, tr("Connection details")));

    QHBoxLayout *buttonsLayout = new QHBoxLayout();


    buttonsLayout->addStretch();

    QPushButton *connectButton = new QPushButton(IconUtil::getIcon("connect"), tr("Connect"));
    connectButton->setToolTip(tr("Connect to database"));
    connectButton->setDefault(true);
    buttonsLayout->addWidget(connectButton);

    QPushButton *testConnectionButton = new QPushButton();
    testConnectionButton->setIcon(IconUtil::getIcon("test_connection"));
    testConnectionButton->setToolTip(tr("Test connection"));
    buttonsLayout->addWidget(testConnectionButton);

    QPushButton *addButton = new QPushButton();
    addButton->setIcon(IconUtil::getIcon("add"));
    addButton->setToolTip(tr("Add new connection"));
    addButton->setEnabled(false);
    buttonsLayout->addWidget(addButton);

    QPushButton *saveButton = new QPushButton();
    saveButton->setIcon(IconUtil::getIcon("save"));
    saveButton->setToolTip(tr("Save connection"));
    buttonsLayout->addWidget(saveButton);

    QPushButton *deleteButton = new QPushButton();
    deleteButton->setIcon(IconUtil::getIcon("delete"));
    deleteButton->setToolTip(tr("Delete connection"));
    deleteButton->setEnabled(false);
    buttonsLayout->addWidget(deleteButton);


    layout->addLayout(buttonsLayout);
    layout->addStretch();

    splitter->addWidget(w);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addConnection()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteConnection()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectToDb()));
    connect(testConnectionButton, SIGNAL(clicked()), this, SLOT(testConnection()));

    connect(connectionEditor, SIGNAL(hasConnection(bool)), addButton, SLOT(setEnabled(bool)));
    connect(connectionEditor, SIGNAL(hasConnection(bool)), deleteButton, SLOT(setEnabled(bool)));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveConnection()));

    new QShortcut(QKeySequence("Delete"), deleteButton, SIGNAL(clicked()));
}

void ConnectionPageConnectWidget::updateCurrentConnection()
{
    if(connectionEditor->validate(false)){

        modifyingConnection = true;

        connectionEditor->updateCurrentConnection();
        model->connectionUpdated(connectionEditor->getCurrentConnection());

        modifyingConnection = false;
    }
}

void ConnectionPageConnectWidget::loadConnectionList()
{
    model->setConnectionList(SettingsHelper::loadConnectionList());
}

void ConnectionPageConnectWidget::saveConnectionList()
{
    const QList<DbConnectionInfo*> connectionList = model->getConnectionList();

    SettingsHelper::saveConnectionList(connectionList);

    Settings::setValue("ConnectionPage/SortIndex", proxyModel->sortColumn());
    Settings::setValue("ConnectionPage/SortOrder", proxyModel->sortOrder()==Qt::AscendingOrder ? 1 : 0);
}

void ConnectionPageConnectWidget::addConnection()
{
    updateCurrentConnection();

    table->clearSelection();
    table->setCurrentIndex(QModelIndex());

    connectionEditor->setCurrentConnection(0);
    connectionEditor->focus();
}

void ConnectionPageConnectWidget::saveConnection()
{
    if(!connectionEditor->validate()){
        return;
    }

    DbConnectionInfo *connection = connectionEditor->getCurrentConnection();
    if(connection){
        updateCurrentConnection();
    }else{
        connection = connectionEditor->createConnection();
        Q_ASSERT(connection);

        model->addConnection(connection);
        selectInTable(connection);
    }

    saveConnectionList();
}

void ConnectionPageConnectWidget::deleteConnection()
{
    if(QMessageBox::question(this->window(), tr("Confirm deletion"),
                             tr("Delete current connection?"),
                              QMessageBox::Ok|QMessageBox::Cancel,
                              QMessageBox::Ok) != QMessageBox::Ok){
        return;
    }

    DbConnectionInfo *connection = connectionEditor->getCurrentConnection();

    table->clearSelection();
    table->setCurrentIndex(QModelIndex());
    connectionEditor->setCurrentConnection(0);

    if(connection != 0){
        model->deleteConnection(connection);
        saveConnectionList();
    }

    table->setFocus();
}

void ConnectionPageConnectWidget::connectToDb(bool test)
{
    if(this->busy){
        return;
    }

    testMode = test;

    DbConnectionInfo *connectionInfo = connectionEditor->getCurrentConnection();

    if(connectionInfo == 0){
        connectionInfo = connectionEditor->createConnection();
        if(connectionInfo == 0){
            return;
        }
        deleteOnFail = true; //just created. will delete if unable to connect
    }else{
        deleteOnFail = false; //existing connection. will not delete

        updateCurrentConnection();
    }

    if(!connectionInfo->username.isEmpty() &&
            connectionInfo->password.isEmpty()){
        PasswordEntryDialog dialog(this->window());
        if(dialog.exec()){
            connectionInfo->password = dialog.password();
            connectionInfo->savePassword = dialog.savePassword();
        }else{
            if(deleteOnFail){
                connectionEditor->deleteCurrentConnection();
            }
            return;
        }
    }

    this->busy = true;
    setEnabled(false);

    emit busyStateChanged(true);

    DbConnection *db = new DbConnection(connectionInfo->title,
                                        connectionInfo->environment,
                                        connectionInfo->connectionString,
                                        connectionInfo->username,
                                        connectionInfo->password,
                                        connectionInfo->connectAs);

    AsyncConnect *asyncConnect = new AsyncConnect(db, 0, QThread::currentThread(), this);
    connect(asyncConnect, SIGNAL(connectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)), this, SLOT(connectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)));
    asyncConnect->start();
}

void ConnectionPageConnectWidget::testConnection()
{
    connectToDb(true);
}

void ConnectionPageConnectWidget::connectToInitialUuid()
{
    int connectionIx = model->indexOf(initialConnectionUuid);
    if(connectionIx != -1){
        QModelIndex proxyIndex = proxyModel->mapFromSource(model->index(connectionIx,0));
        table->setCurrentIndex(proxyIndex);
        table->selectionModel()->select(proxyIndex, QItemSelectionModel::Rows | QItemSelectionModel::SelectCurrent);
        connectToDb();
    }
}

void ConnectionPageConnectWidget::connectionActivated(const QModelIndex &index)
{
    if(!index.isValid()){
        return;
    }

    DbConnectionInfo *connection = model->connectionAt(proxyModel->mapToSource(index).row());
    if(connection){
        connectToDb();
    }
}

void ConnectionPageConnectWidget::connectionEstablished(AsyncConnect *thread, DbConnection *db, void *data, bool error, const OciException &ex)
{
    Q_UNUSED(data);

    thread->wait();
    thread->deleteLater();

    this->busy = false;
    setEnabled(true);

    emit busyStateChanged(false);

    DbConnectionInfo *currentConnection = connectionEditor->getCurrentConnection();

    if(error){
        delete db;

        QMessageBox::critical(this->window(), tr("Failed to connect"),
                              tr("Failed to connect to %1\n%2").arg(currentConnection->title, ex.getErrorMessage()));

        if(deleteOnFail){
            connectionEditor->deleteCurrentConnection();
        }

        selectInTable(currentConnection);
    }else{
        if(deleteOnFail){ //new connection
            model->addConnection(currentConnection);
        }

        saveConnectionList();

        if(testMode){
            delete db;

            QMessageBox::information(this->window(), tr("Connected"), tr("Connected"));

            selectInTable(currentConnection);
        }else{
            emit connected(db, currentConnection);
        }
    }
}

void ConnectionPageConnectWidget::filterList(const QString &pattern)
{
    proxyModel->setDynamicSortFilter(false);

    proxyModel->setFilterWildcard(pattern);

    proxyModel->setDynamicSortFilter(true);
}

void ConnectionPageConnectWidget::resizeEvent(QResizeEvent *)
{
    table->horizontalHeader()->setDefaultSectionSize(qCeil(table->width() / (float) 3));
}

bool ConnectionPageConnectWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == filterEditor->lineEdit() && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(keyEvent->key() == Qt::Key_Down){
            selectConnection(true);
            return true;
        }else if(keyEvent->key() == Qt::Key_Up){
            selectConnection(false);
            return true;
        }else{
            return false;
        }
    }else{
        return QWidget::eventFilter(obj, event);
    }
}

void ConnectionPageConnectWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter ||
            event->key() == Qt::Key_Return){
        connectToDb();
    }else{
        QWidget::keyPressEvent(event);
    }
}

void ConnectionPageConnectWidget::currentConnectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if(!current.isValid()){
        return;
    }

    if(connectionEditor->getCurrentConnection() != 0){
        updateCurrentConnection();
    }

    DbConnectionInfo *connection = model->connectionAt(proxyModel->mapToSource(current).row());
    if(connection){
        connectionEditor->setCurrentConnection(connection);
    }
}

void ConnectionPageConnectWidget::connectionModified(DbConnectionInfo *connection)
{
    if(!modifyingConnection && connectionEditor->getCurrentConnection() == connection){
        connectionEditor->setCurrentConnection(connection);
    }
}

void ConnectionPageConnectWidget::focusReady()
{
    if(!table->selectionModel()->selectedIndexes().isEmpty()){
        return;
    }

    selectConnection(true);
}

void ConnectionPageConnectWidget::selectConnection(bool first)
{
    table->setFocus();

    if(proxyModel->rowCount() == 0){
        connectionEditor->focus();
        return;
    }

    QModelIndex index = proxyModel->index(first ? 0 : proxyModel->rowCount()-1, 0);
    table->setCurrentIndex(index);
    table->selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::SelectCurrent);
}

void ConnectionPageConnectWidget::selectInTable(DbConnectionInfo *connection)
{
    QModelIndex index = proxyModel->mapFromSource(model->index(model->indexOf(connection),0));
    if(index.isValid()){
        table->setFocus();
        table->selectRow(index.row());
    }else{
        connectionEditor->focus();
    }
}
