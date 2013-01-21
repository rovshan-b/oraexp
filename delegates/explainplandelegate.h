#ifndef EXPLAINPLANDELEGATE_H
#define EXPLAINPLANDELEGATE_H

#include <QStyledItemDelegate>

class ExplainPlanDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ExplainPlanDelegate(QObject *parent = 0);

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    
signals:
    
public slots:
    
};

#endif // EXPLAINPLANDELEGATE_H
