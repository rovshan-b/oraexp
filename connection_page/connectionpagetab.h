#ifndef CONNECTIONPAGETAB_H
#define CONNECTIONPAGETAB_H

#include <QWidget>
#include "connectionpageobject.h"

class QLabel;
class DbUiManager;

class ConnectionPageTab : public QWidget, public ConnectionPageObject
{
    Q_OBJECT
public:
    explicit ConnectionPageTab(DbUiManager *uiManager, QWidget *parent = 0);

    virtual QObject *getQueryEndMonitor() {return this;}

    virtual bool canOpen() const {return true;}
    virtual bool canSave() const {return false;}
    virtual bool canPrint() const {return false;}

    virtual void focusAvailable() {}

    void setProperties(const QHash<QString,QString> &properties);
    QString propertyValue(const QString &propName) const;

    QString getTabId() const;
    void setTabId(const QString &tabId);

signals:
    void busyStateChanged(ConnectionPageObject *tab, bool busy);
    void stateChanged();

    void initCompleted(ConnectionPageObject *tab);

public slots:
    virtual void queryExecTaskCompleted(const QString &taskName);
    virtual void emitBusyStateChangedSignal();
    virtual void emitInitCompletedSignal();

protected:
    DbUiManager *uiManager;

    QHash<QString,QString> properties;

    //returns all tabs of same type on same connection
    QList<ConnectionPageTab *> getPeerTabs(int limit = -1) const;

    QString tabId;

};

#endif // CONNECTIONPAGETAB_H
