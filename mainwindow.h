#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConnectionsPane;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void showConnectDialog();

protected:
    virtual void closeEvent(QCloseEvent *event);
    //virtual bool eventFilter(QObject *obj, QEvent *ev);

private:
    Ui::MainWindow *ui;
    void createMenu();
    ConnectionsPane *connectionsPane;
};

#endif // MAINWINDOW_H
