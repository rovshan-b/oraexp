#include "codereparser.h"
#include "code_parser/codeparserthread.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "numutil.h"

CodeReparser::CodeReparser(QObject *parent) :
    QObject(parent),
    parserThread(0),
    requestId(0)
{
}

CodeReparser::~CodeReparser()
{
    stopThread();
}

void CodeReparser::parse(const QString &code)
{
    stopThread();

    parserThread = new CodeParserThread(code, NumUtil::increase(requestId));
    connect(parserThread, SIGNAL(parsingCompleted(int,bool,PlSqlTreeBuilder*,int)), this, SLOT(parsed(int,bool,PlSqlTreeBuilder*,int)));
    parserThread->start(QThread::IdlePriority);
}

void CodeReparser::stopThread()
{
    if(parserThread){
        parserThread->stop();
        parserThread->wait();
        parserThread->deleteLater();
        parserThread = 0;
    }
}

void CodeReparser::parsed(int requestId, bool success, PlSqlTreeBuilder *treeBulder, int elapsedTime)
{
    if(requestId != this->requestId){
        delete treeBulder;
    }else{
        emit parsingCompleted(requestId, success, treeBulder, elapsedTime);
    }
}
