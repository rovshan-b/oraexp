#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../ireducelistener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IReduceListener
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void reduced(int ruleId, int symbolCount, ParsingTable *parsingTable);

protected:
    void closeEvent ( QCloseEvent * event );
    
private slots:
    void on_actionScan_triggered();

    void on_actionParse_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
