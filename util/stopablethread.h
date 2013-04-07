#ifndef STOPABLETHREAD_H
#define STOPABLETHREAD_H

#include <QThread>

class StopableThread : public QThread
{
    Q_OBJECT
public:
    explicit StopableThread(QObject *parent);
    virtual ~StopableThread(){}

    void stop() {this->stopped=true;}
    
protected:
    bool stopped;
    
};

#endif // STOPABLETHREAD_H
