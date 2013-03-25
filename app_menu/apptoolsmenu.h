#ifndef APPTOOLSMENU_H
#define APPTOOLSMENU_H

#include "appmainmenu.h"
#include "connection_page/dbuimanager.h"

class QMenu;
class QToolBar;
class QAction;

class AppToolsMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppToolsMenu(QMenu *toolsMenu, QToolBar *toolbar, QObject *parent);

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

private:
    void setupMenu(QMenu *toolsMenu, QToolBar *toolbar);

    QAction *toolsCompareSchemasAction;

    QAction *toolsCompareDataAction;
    QAction *toolsCopyDataAction;

    QAction *toolsOptionsAction;

private slots:
    void addSchemaComparer(){uiManager()->addSchemaComparer();}
    void addDataComparer(){uiManager()->addDataComparer();}
    void addDataCopier(){uiManager()->addDataCopier();}

};

#endif // APPTOOLSMENU_H
