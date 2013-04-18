#ifndef RESULTSETDELETER_H
#define RESULTSETDELETER_H

#include <QRunnable>

class Resultset;

class ResultsetDeleter : public QRunnable
{
public:
    ResultsetDeleter(Resultset *rs);

    virtual void run();
private:
    Resultset *rs;
};

#endif // RESULTSETDELETER_H
