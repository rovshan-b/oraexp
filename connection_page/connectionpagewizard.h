#ifndef CONNECTIONPAGEWIZARD_H
#define CONNECTIONPAGEWIZARD_H

#include <QWizard>
#include "connectionpageobject.h"

class ConnectionPageWizard : public QWizard, public ConnectionPageObject
{
    Q_OBJECT
public:
    explicit ConnectionPageWizard(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~ConnectionPageWizard();
    
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

private:
    bool canClose();
    
};

#endif // CONNECTIONPAGEWIZARD_H
