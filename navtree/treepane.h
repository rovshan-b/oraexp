#ifndef TREEPANE_H
#define TREEPANE_H

#include "connection_page/connectionpagetab.h"

class DbTreeView;
class QToolButton;
class IQueryScheduler;
class DbUiManager;

class TreePane : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit TreePane(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

    virtual QSize sizeHint () const;

private:
    DbTreeView *dbTree;
    //QToolButton *refreshButton;

    void populateTree();

};

#endif // TREEPANE_H
