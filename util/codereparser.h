#ifndef CODEREPARSER_H
#define CODEREPARSER_H

#include <QObject>

class CodeParserThread;
class PlSqlTreeBuilder;

class CodeReparser : public QObject
{
    Q_OBJECT
public:
    explicit CodeReparser(QObject *parent = 0);
    ~CodeReparser();
    
    void parse(const QString &code);

signals:
    void parsingCompleted(int requestId,
                          bool success,
                          PlSqlTreeBuilder *treeBulder,
                          int elapsedTime);

private slots:
    void parsed(int requestId,
                bool success,
                PlSqlTreeBuilder *treeBulder,
                int elapsedTime);

private:
    CodeParserThread *parserThread;

    void stopThread();

    //will increase request id by 1 and start parsing.
    //when parsing is completed will check if completed parsing task's id is the same as current requestId.
    //if so it is current request and will emit parsing completed signal
    int requestId;
    
};

#endif // CODEREPARSER_H
