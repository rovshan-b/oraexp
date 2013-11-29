#ifndef CONNECTIONPAGEWINDOW_H
#define CONNECTIONPAGEWINDOW_H

#include <QDialog>
#include "connectionpagewindowobject.h"

class ConnectionPageWindow : public QDialog, public ConnectionPageWindowObject
{
    Q_OBJECT
public:
    explicit ConnectionPageWindow(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~ConnectionPageWindow();

    virtual QObject *getQueryEndMonitor() {return this;}

    virtual QString getDisplayName() const;
    virtual QIcon getIcon() const;
    
signals:
    void busyStateChanged(ConnectionPageObject *tab, bool busy);
    void initCompleted(ConnectionPageObject *tab);

public slots:
    virtual void queryExecTaskCompleted(const QString &taskName);
    virtual void emitBusyStateChangedSignal();
    virtual void emitInitCompletedSignal();

    void makeVisible();
    void makeVisibleIfInProgress();

    virtual void reject ();

protected:
    virtual void closeEvent(QCloseEvent *e);
    virtual bool canClose();

    virtual void setInProgress(bool inProgress);
    bool isInProgress() const;

private:
    bool inProgress;
    
};

#endif // CONNECTIONPAGEWINDOW_H
