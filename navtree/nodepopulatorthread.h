#ifndef NODEPOPULATORTHREAD_H
#define NODEPOPULATORTHREAD_H

#include <QThread>
#include <QModelIndex>
#include "dbtreeitemresult.h"
class DbTreeItem;

class NodePopulatorThread : public QThread
{
    Q_OBJECT
public:
    explicit NodePopulatorThread(const QModelIndex &parentNode, QObject *parent);
    virtual ~NodePopulatorThread();

protected:
    virtual void run();

signals:
    void queryCompleted(const DbTreeItemResult &result);

private:
    QModelIndex parentNode;

};

#endif // NODEPOPULATORTHREAD_H
