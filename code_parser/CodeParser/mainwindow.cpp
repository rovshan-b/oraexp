#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../plsql/plsqlscanner.h"
#include "../plsql/plsqltokens.h"
#include "../textcursorreader.h"
#include "../codeparserfactory.h"
#include "../codeparser.h"
#include "../parsingtable.h"
#include "../plsql/plsqlparsingtable.h"
#include "../plsql/plsqltreebuilder.h"
#include "beans/parsetreenode.h"
#include "beans/tokeninfo.h"
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    ui->codeEditor->setPlainText(settings.value("code").toString());

    model = new QStandardItemModel(this);
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);
    ui->treeView->header()->setStretchLastSection(false);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(nodeActivated(QModelIndex)));
    connect(ui->codeEditor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
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
        /*if(token==PLS_E_O_F){
            s="EOF";
        }else{
            s=QString("%1 (%2) (pos: %3, end pos: %4, start line: %5, end line: %6, start line pos: %7, end line pos: %8)").
                    arg(scanner.getTokenLexeme()).arg(token).
                    arg(scanner.getTokenStartPos()).arg(scanner.getTokenEndPos()).
                    arg(scanner.getTokenStartLine()).arg(scanner.getTokenEndLine()).
                    arg(scanner.getTokenStartLinePos()).arg(scanner.getTokenEndLinePos());
        }
        ui->output->appendPlainText(s);*/
    }while(token!=PLS_E_O_F);

    QString title=QString("scanned in %1 ms").arg(t.elapsed());
    setWindowTitle(title);
}

void MainWindow::on_actionParse_triggered()
{
    parseCurrent(true);
}

void MainWindow::buildTree(ParseTreeNode *rootNode)
{
    model->clear();
    addChildNodes(rootNode, model->invisibleRootItem());
    ui->treeView->expandAll();
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
}

void MainWindow::addChildNodes(ParseTreeNode *parseTreeNode, QStandardItem *treeViewNode)
{
    QString nodeTitle;
    if(parseTreeNode->tokenInfo==0){
        nodeTitle = "Root node";
    }else if(parseTreeNode->tokenInfo->tokenType == TokenInfo::Rule){
        nodeTitle = PlSqlParsingTable::getInstance()->getRuleName(parseTreeNode->tokenInfo->tokenOrRuleId);
    }else{
        nodeTitle = parseTreeNode->tokenInfo->lexeme;
    }

    QStandardItem *newItem = new QStandardItem(nodeTitle);
    newItem->setData(QString("%1:%2").arg(parseTreeNode->tokenInfo->startPos).arg(parseTreeNode->tokenInfo->endPos));
    treeViewNode->appendRow(newItem);

    for(int i=0; i<parseTreeNode->children.size(); ++i){
        addChildNodes(parseTreeNode->children.at(i), newItem);
    }
}

void MainWindow::nodeActivated(const QModelIndex &index)
{
    if(!index.isValid()){
        return;
    }
    QStandardItem *item = model->itemFromIndex(index);
    if(!item){
        return;
    }

    QString data = item->data().toString();
    QList<QString> parts = data.split(':');
    Q_ASSERT(parts.size()==2);
    QTextCursor cur = ui->codeEditor->textCursor();
    cur.setPosition(parts.at(0).toInt());
    cur.setPosition(parts.at(1).toInt(), QTextCursor::KeepAnchor);
    ui->codeEditor->setTextCursor(cur);
}

void MainWindow::cursorPositionChanged()
{
    ui->statusBar->showMessage(QString("Cursor position: %1").arg(ui->codeEditor->textCursor().position()));
}

bool MainWindow::parseCurrent(bool constructDisplayTree)
{
    ui->output->clear();

    TextCursorReader* reader=new TextCursorReader(ui->codeEditor->textCursor());
    CodeParser *parser=CodeParserFactory::createParser("plsql", reader);
    PlSqlTreeBuilder treeBulder;
    parser->setReduceListener(&treeBulder);

    QTime t;
    t.start();

    bool success=parser->parse();

    int elapsed = t.elapsed();

    if(!success){
        ui->output->appendPlainText(QString("Parse completed with error on positon: %1").arg(parser->getScanner()->getTokenStartPos()));
        ui->output->appendPlainText("expected one of following:");

        QPair< QList<int>, QList<int> > expectedTokens=parser->getExpectedTokens();
        QList<int> tokens = expectedTokens.first;
        QList<int> rules = expectedTokens.second;

        ui->output->appendPlainText("tokens:");
        QString tokenDesc;
        for(int i=0; i<tokens.size(); ++i){
            int token=tokens.at(i);
            if(token<NON_LITERAL_START_IX){
                tokenDesc=parser->getParsingTable()->keywords.at(token);
            }else{
                tokenDesc=QString::number(token);
            }
            ui->output->appendPlainText(tokenDesc);
        }

        ui->output->appendPlainText("rules:");
        for(int i=0; i<rules.size(); ++i){
            ui->output->appendPlainText(QString::number(rules.at(i)));
        }
    }else{
        ui->output->appendPlainText(QString("Successfully completed parsing in %1 ms").arg(elapsed));
    }

    if(constructDisplayTree){
        buildTree(treeBulder.getRootNode());
    }


    /*
    QList<int> objectNamePath = QList<int>() << R_START_RULE << R_CREATE_OBJECT << R_OBJ_TYPE_AND_NAME_AND_BODY << R_OBJECT_NAME;
    ParseTreeNode *objectNameNode = treeBulder.getNode(objectNamePath);

    if(objectNameNode){
        QString objectName;

        if(objectNameNode->children.size()==1){
            objectName = objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme;
        }else{
            Q_ASSERT(objectNameNode->children.size()==3);
            objectName = QString("%1.%2").arg(objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme,
                                              objectNameNode->children.at(2)->children.at(0)->tokenInfo->lexeme);
            ui->output->appendPlainText(QString("Object name is: %1").arg(objectName));
        }
    }*/

    delete parser;

    return success;
}

void MainWindow::on_actionParse_All_triggered()
{
    QString path = "/home/rovshan/Projects/Qt/OraExp/code_parser/CodeParser/tests/";
    QDir dir(path);
    QStringList files = dir.entryList(QStringList() << "*.sql");
    int parsedCount = 0;
    bool allSucceeded=true;
    bool lastParseSucceeded=false;
    foreach(const QString &file, files){
        QFile f(QString("%1%2").arg(path, file));
        if(!f.open(QIODevice::ReadOnly)){
            QMessageBox::information(this, "file", f.errorString());
            break;
        }

        ui->codeEditor->setPlainText(f.readAll());
        setWindowTitle(file);

        lastParseSucceeded = parseCurrent(false);

        if(!lastParseSucceeded){
            //break;
            qDebug() << "Failed to parse" << file;
            allSucceeded = false;
        }else{
            ++parsedCount;
        }
    }

    setWindowTitle(QString("%1 - %2 files parsed").arg(windowTitle()).arg(parsedCount));

    if(allSucceeded){
        setWindowTitle(QString("%1 - All files parsed").arg(windowTitle()));
    }
}
