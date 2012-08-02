#ifndef CONNECTIONPAGETAB_H
#define CONNECTIONPAGETAB_H

#include <QWidget>
#include "util/queryscheduler.h"
#include "interfaces/isearchable.h"

class QLabel;
class DbConnection;
class DbUiManager;

class ConnectionPageTab : public QWidget, public QueryScheduler, public ISearchable
{
    Q_OBJECT
public:
    explicit ConnectionPageTab(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~ConnectionPageTab();

    //this function can be used to create user interface if tab wants
    //to allow inherited classes to override ui creation logic
    virtual void createUi(){}

    bool needsSeparateConnection(){return  requiresSeparateConnection;}
    virtual bool disableWhileConnecting(){return true;}
    virtual void setConnection(DbConnection *db);

    virtual QObject *getQueryEndMonitor() {return this;}
    virtual DbConnection *getDb() const {return this->db;}
    virtual void increaseRefCount();
    virtual void decreaseRefCount();

    bool isBusy() const;
    void setBusy(bool busy);

    virtual bool canOpen() const {return false;}
    virtual bool canSave() const {return false;}
    virtual bool canPrint() const {return false;}

    virtual bool canFind() const {return false;}
    virtual bool canFindNext() const {return canFind();}
    virtual bool canFindPrevious() const {return canFind();}

    virtual void open() {}

    virtual void showSearchWidget() {}
    virtual void findNext() {}
    virtual void findPrevious() {}

signals:
    void tabBusyStateChanged(ConnectionPageTab *tab, bool busy);
    void stateChanged();

public slots:
    void queryExecTaskCompleted(const QString &taskName);

protected:
    DbUiManager *uiManager;
    DbConnection *db;
    bool requiresSeparateConnection;
    bool busy;

    void beforeEnqueueQuery();

private:
    int activeQueryCount;

};

#endif // CONNECTIONPAGETAB_H
