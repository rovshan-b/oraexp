#ifndef CONNECTIONPAGETAB_H
#define CONNECTIONPAGETAB_H

#include <QWidget>
#include <QTextStream>
#include "connectionpageobject.h"

class QLabel;

class ConnectionPageTab : public QWidget, public ConnectionPageObject
{
    Q_OBJECT
public:
    explicit ConnectionPageTab(DbUiManager *uiManager, QWidget *parent = 0);

    virtual QObject *getQueryEndMonitor() {return this;}

    virtual bool canSave() const {return false;}
    virtual bool canPrint() const {return false;}

    virtual void focusAvailable() {}

    void setProperties(const QHash<QString,QString> &properties);
    QString propertyValue(const QString &propName) const;

    QString getTitle() const;
    void setTitle(const QString &title);

    QString getTabId() const;
    void setTabId(const QString &tabId);

    virtual bool isModified() const {return false;}
    virtual void setModified(bool modified) {Q_UNUSED(modified);}

    virtual QString getCurrentFileName() const {return "";}
    virtual void setCurrentFileName(const QString &fileName) {Q_UNUSED(fileName);}

signals:
    void busyStateChanged(ConnectionPageObject *tab, bool busy);
    void stateChanged();

    void initCompleted(ConnectionPageObject *tab);
    void captionChanged(ConnectionPageTab *tab, const QString &caption);

public slots:
    virtual void queryExecTaskCompleted(const QString &taskName);
    virtual void emitBusyStateChangedSignal();
    virtual void emitInitCompletedSignal();

    void saveContents();
    void saveContentsAs();

protected:
    QHash<QString,QString> properties;

    //returns all tabs of same type on same connection
    QList<ConnectionPageTab *> getPeerTabs(int limit = -1) const;

    QString tabTitile;
    QString tabId;

    virtual void saveToStream(QTextStream &out) {Q_UNUSED(out);}

    void setCaption(const QString &caption);
    void setModifiedStatusToCaption(bool changed);

private:
    void saveToFile(const QString &fileName);

};

#endif // CONNECTIONPAGETAB_H
