#ifndef CHECKBOXLISTDELEGATE_H
#define CHECKBOXLISTDELEGATE_H

#include <QStyledItemDelegate>

class CheckBoxListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckBoxListDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // CHECKBOXLISTDELEGATE_H
