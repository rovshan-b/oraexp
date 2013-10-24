#ifndef APPHELPMENU_H
#define APPHELPMENU_H

#include <QObject>

class QMenu;
class QToolBar;
class QAction;

class AppHelpMenu : public QObject
{
    Q_OBJECT
public:
    explicit AppHelpMenu(QMenu *helpMenu, QToolBar *toolbar, QObject *parent);

private slots:
    void displayHelpContents();
    void displayAboutBox();

private:
    void setupMenu(QMenu *helpMenu, QToolBar *toolbar);

    QAction *helpContentsAction;
    QAction *helpAboutAction;
};

#endif // APPHELPMENU_H
