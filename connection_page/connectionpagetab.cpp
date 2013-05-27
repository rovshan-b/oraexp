#include "connectionpagetab.h"
#include "connection_page/dbuimanager.h"
#include "util/dialoghelper.h"
#include <QtGui>

#include <iostream>
using namespace std;

ConnectionPageTab::ConnectionPageTab(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent), ConnectionPageObject(uiManager)
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

QString ConnectionPageTab::getTitle() const
{
    return this->tabTitile;
}

void ConnectionPageTab::setTitle(const QString &title)
{
    this->tabTitile = title;
}

QString ConnectionPageTab::getTabId() const
{
    return this->tabId;
}

void ConnectionPageTab::setTabId(const QString &tabId)
{
    this->tabId = tabId;
}

QList<ConnectionPageTab *> ConnectionPageTab::getPeerTabs(int limit) const
{
    return uiManager->getTabsByConnection(getDb(), this->metaObject()->className(), limit);
}

void ConnectionPageTab::setCaption(const QString &caption)
{
    emit captionChanged(this, caption);
}

void ConnectionPageTab::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageTab::saveContents()
{
    QString fileName = getCurrentFileName();

    if(!fileName.isEmpty()){
        saveToFile(fileName);
    }else{
        saveContentsAs();
    }
}

void ConnectionPageTab::saveContentsAs()
{
    QString filenameToSuggest = getCurrentFileName();
    if(filenameToSuggest.isEmpty()){
        filenameToSuggest = QString("%1.sql").arg(getTitle());
    }
    QString fileName = DialogHelper::showFileSaveDialog(this->window(), filenameToSuggest, "sql");
    if(fileName.isEmpty()){
        return;
    }

    saveToFile(fileName);
    setCurrentFileName(fileName);
}

void ConnectionPageTab::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this->window(),
                              tr("Save failed"),
                              tr("Failed to save file. %1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    saveToStream(out);
    out.flush();
    file.close();

    setModified(false);
}

void ConnectionPageTab::setModifiedStatusToCaption(bool changed)
{
    QString caption = getTitle();
    if(changed){
        caption.append("*");
    }

    setCaption(caption);
}

void ConnectionPageTab::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}

void ConnectionPageTab::emitBusyStateChangedSignal()
{
    emit busyStateChanged(this, busy);
}
