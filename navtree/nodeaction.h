#ifndef QNODEACTION_H
#define QNODEACTION_H

#include <QAction>
#include <QModelIndex>

class NodeAction : public QAction
{
    Q_OBJECT
public:
    explicit NodeAction(const QModelIndex &index, const QIcon &icon, const QString &text, QObject *parent = 0);

    QModelIndex getModelIndex() const;

signals:

public slots:

private:
    QModelIndex index;

};

#endif // QNODEACTION_H
