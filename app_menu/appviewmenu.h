#ifndef APPVIEWMENU_H
#define APPVIEWMENU_H

#include <QObject>

class QMenu;
class QToolBar;
class QAction;

class AppViewMenu : public QObject
{
    Q_OBJECT
public:
    explicit AppViewMenu(QMenu *viewMenu, QToolBar *toolbar, QObject *parent);
    ~AppViewMenu();

private slots:
    void setApplicationStyle();

private:
    void setupMenu(QMenu *viewMenu, QToolBar *toolbar);

    QAction *viewDatabaseObjectsAction;
    QAction *viewApplicationStyleAction;

    QMenu *viewAppStyleMenu;
    void createAppStyleMenu();

};

#endif // APPVIEWMENU_H
