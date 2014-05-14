#ifndef CODEPARSERTHREAD_H
#define CODEPARSERTHREAD_H

#include "util/stopablethread.h"

class CodeParser;
class PlSqlTreeBuilder;

class CodeParserThread : public StopableThread
{
    Q_OBJECT
public:
    explicit CodeParserThread(const QString &code, int requestId, QObject *parent = 0);
    virtual ~CodeParserThread();

    virtual void stop();

signals:
    void parsingCompleted(int requestId,
                          bool success,
                          PlSqlTreeBuilder *treeBulder,
                          int elapsedTime);

protected:
    virtual void run();
    
private:
    QString code;
    int requestId;

    CodeParser *parser;
    
};

#endif // CODEPARSERTHREAD_H
