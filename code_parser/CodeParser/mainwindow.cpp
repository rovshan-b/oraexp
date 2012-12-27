#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../plsql/plsqlscanner.h"
#include "../plsql/plsqltokens.h"
#include "../textcursorreader.h"
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    ui->codeEditor->setPlainText(settings.value("code").toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings settings;
    settings.setValue("code", ui->codeEditor->toPlainText());
}

void MainWindow::on_actionScan_triggered()
{
    ui->output->clear();

    TextCursorReader* reader=new TextCursorReader(ui->codeEditor->textCursor());
    PlSqlScanner scanner(reader); //scanner will delete reader in destructor
    int token;
    QString s;

    QTime t;
    t.start();

    do{
        token=scanner.getNextToken();
        if(token==PLS_E_O_F){
            s="EOF";
        }else{
            s=QString("%1 (%2) (pos: %3, end pos: %4, start line: %5, end line: %6, start line pos: %7, end line pos: %8)").
                    arg(scanner.currentLexeme).arg(token).
                    arg(scanner.getTokenStartPos()).arg(scanner.getTokenEndPos()).
                    arg(scanner.getTokenStartLine()).arg(scanner.getTokenEndLine()).
                    arg(scanner.getTokenStartLinePos()).arg(scanner.getTokenEndLinePos());
        }
        ui->output->appendPlainText(s);
    }while(token!=PLS_ERROR && token!=PLS_E_O_F);

    QString title=QString("scanned in %1 ms").arg(t.elapsed());
    setWindowTitle(title);
}
