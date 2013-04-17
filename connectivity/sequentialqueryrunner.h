#ifndef SEQUENTIALQUERYRUNNER_H
#define SEQUENTIALQUERYRUNNER_H

#include <QObject>
#include <QPair>
#include <QStringList>
#include "connectivity/queryresult.h"
#include "dialogs/sequantialexecutionerrordialog.h"

class IQueryScheduler;

class SequentialQueryRunner : public QObject
{
    Q_OBJECT
public:
    explicit SequentialQueryRunner(QObject *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void execute(const QString &query, QWidget *parentWidget);

    bool isBusy() const {return this->busy;}
    void stop();

    int getCurrentQueryPos() const {return this->currentQueryStartPos;}

signals:
    void beforeExecute(const QString &query, int startPos, int endPos);
    void queryResultAvailable(const QueryResult &result);
    void completed();

private slots:
    void queryCompleted(const QueryResult &result);

private:
    IQueryScheduler *queryScheduler;
    QStringList queries;
    QList< QPair<int,int> > queryPositions;

    QString currentQuery;
    int currentQueryStartPos;

    QWidget *parentWidget;

    SequantialExecutionErrorDialog::ErrorAction lastErrorAction;
    bool ignoreAllErrors;
    QList<int> errorCodesToIgnore;

    bool busy;
    bool stopped;

    void executeNextQuery();

    void emitCompletedSignal();
    
};

#endif // SEQUENTIALQUERYRUNNER_H
