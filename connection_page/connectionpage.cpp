#include "connectionpage.h"
#include "connectionpageconnectwidget.h"
#include "connectionpageconnectedwidget.h"
#include <QtGui>

ConnectionPage::ConnectionPage(QWidget *parent) :
    QWidget(parent),
    mainWidget(0),
    uiManager(this)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    connectWidget = new ConnectionPageConnectWidget();
    mainLayout->addWidget(connectWidget);

    setLayout(mainLayout);
}

ConnectionPage::~ConnectionPage()
{

}

bool ConnectionPage::isConnected() const
{
    return (mainWidget != 0);
}

DbUiManager *ConnectionPage::getUiManager()
{
    return &this->uiManager;
}

void ConnectionPage::addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title)
{
    Q_ASSERT(mainWidget);

    mainWidget->addTab(tab, icon, title);
}

void ConnectionPage::addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title)
{
    Q_ASSERT(mainWidget);

    mainWidget->addWindow(window, icon, title);
}

void ConnectionPage::closeTab(QWidget *widget)
{
    Q_ASSERT(mainWidget);

    mainWidget->closeTab(widget);
}

ConnectionPageTab *ConnectionPage::currentConnectionPageTab() const
{
    if(mainWidget){
        return mainWidget->currentConnectionPageTab();
    }else{
        return 0;
    }
}

int ConnectionPage::tabCount() const
{
    if(mainWidget){
        return mainWidget->tabCount();
    }else{
        return 0;
    }
}

ConnectionPageTab *ConnectionPage::tabAt(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabAt(index);
}

QIcon ConnectionPage::tabIcon(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabIcon(index);
}

QString ConnectionPage::tabText(int index) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->tabText(index);
}

bool ConnectionPage::isTreePaneVisible() const
{
    Q_ASSERT(mainWidget);

    return mainWidget->isTreePaneVisible();
}

QList<CtrlTabData *> ConnectionPage::getCtrlTabData() const
{
    if(mainWidget){
        return mainWidget->getCtrlTabData();
    }else{
        return QList<CtrlTabData*>();
    }
}

void ConnectionPage::setCurrentTab(QWidget *widget)
{
    Q_ASSERT(mainWidget);

    mainWidget->setCurrentTab(widget);
}

ConnectionPageTab *ConnectionPage::findTabById(const QString &tabId) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->findTabById(tabId);
}

QList<ConnectionPageTab *> ConnectionPage::getTabsByType(const QString &className) const
{
    Q_ASSERT(mainWidget);

    return mainWidget->getTabsByType(className);
}

QList<ConnectionPageTab *> ConnectionPage::getTabsByConnection(DbConnection *db, const QString &className, int limit)
{
    Q_ASSERT(mainWidget);

    return mainWidget->getTabsByConnection(db, className, limit);
}

void ConnectionPage::closeTab(int index)
{
    Q_ASSERT(mainWidget);

    mainWidget->closeTab(index);
}

void ConnectionPage::toggleTreePane()
{
    Q_ASSERT(mainWidget);

    mainWidget->toggleTreePane();
}

void ConnectionPage::changeTabCaption(ConnectionPageTab *tab, const QString &caption)
{
    Q_ASSERT(mainWidget);

    mainWidget->changeTabCaption(tab, caption);
}
