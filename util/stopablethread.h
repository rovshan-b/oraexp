#ifndef STOPABLETHREAD_H
#define STOPABLETHREAD_H

#include <QThread>

class StopableThread : public QThread
{
    Q_OBJECT
public:
    explicit StopableThread(QObject *parent);
    virtual ~StopableThread(){}

    virtual void stop() {this->stopped=true;}
    bool isStopped() const {return this->stopped;}
    
protected:
    bool stopped;
    
};

#endif // STOPABLETHREAD_H
