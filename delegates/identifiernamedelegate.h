#ifndef IDENTIFIERNAMEDELEGATE_H
#define IDENTIFIERNAMEDELEGATE_H

#include <QStyledItemDelegate>

class DbConnection;

class IdentifierNameDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit IdentifierNameDelegate(DbConnection *db, QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setConnection(DbConnection *db);

private:
    DbConnection *db;
};

#endif // IDENTIFIERNAMEDELEGATE_H
