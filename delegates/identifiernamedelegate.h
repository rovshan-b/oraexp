#ifndef IDENTIFIERNAMEDELEGATE_H
#define IDENTIFIERNAMEDELEGATE_H

#include <QStyledItemDelegate>

class DbConnection;

class IdentifierNameDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit IdentifierNameDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // IDENTIFIERNAMEDELEGATE_H
