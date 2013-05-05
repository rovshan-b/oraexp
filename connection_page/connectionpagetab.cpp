#include "connectionpagetab.h"
#include "connection_page/dbuimanager.h"
#include <QtGui>

#include <iostream>
using namespace std;

ConnectionPageTab::ConnectionPageTab(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent), uiManager(uiManager)
{
}

void ConnectionPageTab::setProperties(const QHash<QString, QString> &properties)
{
    this->properties=properties;
}

QString ConnectionPageTab::propertyValue(const QString &propName) const
{
    return this->properties.value(propName);
}

QList<ConnectionPageTab *> ConnectionPageTab::getPeerTabs(int limit) const
{
    return uiManager->getTabsByConnection(getDb(), this->metaObject()->className(), limit);
}

void ConnectionPageTab::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageTab::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}

void ConnectionPageTab::emitBusyStateChangedSignal()
{
    emit busyStateChanged(this, busy);
}
