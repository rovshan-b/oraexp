#include "codeparserthread.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqlparser.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include <QTime>
#include <QDebug>

CodeParserThread::CodeParserThread(const QString &code, int requestId, QObject *parent) :
    StopableThread(parent),
    code(code),
    requestId(requestId)
{
    StringReader *reader = new StringReader(code);
    parser=new PlSqlParser(new PlSqlScanner(reader));
}

CodeParserThread::~CodeParserThread()
{
    delete parser;
}

void CodeParserThread::stop()
{
    StopableThread::stop();

    parser->stop();
}

void CodeParserThread::run()
{
    PlSqlTreeBuilder *treeBulder = new PlSqlTreeBuilder();
    treeBulder->setCalculateCollapsePositions();
    parser->setReduceListener(treeBulder);

    QTime t;
    t.start();

    bool success=parser->parse();

    int elapsed = t.elapsed();
    qDebug() << "code parsed in" << elapsed << "ms" << ", thread priority was" << priority() << ", number of collapse positions is" << treeBulder->getCollapsePositions().size();

    emit parsingCompleted(requestId, success, treeBulder, elapsed);

    /*
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
    }*/
}
