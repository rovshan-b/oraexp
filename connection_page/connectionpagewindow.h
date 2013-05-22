#ifndef CONNECTIONPAGEWINDOW_H
#define CONNECTIONPAGEWINDOW_H

#include <QDialog>
#include "connectionpageobject.h"

class ConnectionPageWindow : public QDialog, public ConnectionPageObject
{
    Q_OBJECT
public:
    explicit ConnectionPageWindow(QWidget *parent = 0);
    ~ConnectionPageWindow();

    virtual QObject *getQueryEndMonitor() {return this;}
    
signals:
    void busyStateChanged(ConnectionPageObject *tab, bool busy);
    void initCompleted(ConnectionPageObject *tab);

public slots:
    virtual void queryExecTaskCompleted(const QString &taskName);
    virtual void emitBusyStateChangedSignal();
    virtual void emitInitCompletedSignal();

    virtual void reject ();

protected:
    virtual void closeEvent(QCloseEvent *e);
    virtual bool canClose();
    
};

#endif // CONNECTIONPAGEWINDOW_H
