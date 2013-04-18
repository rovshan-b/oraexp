#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class GenericEditableTableModel;

class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(QObject *parent,
                              int modelColumnIx,
                              bool isEditable=true,
                              bool convertToUpperCase=true);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const;


protected:
    int modelColumnIx;
    bool isEditable;
    bool convertToUpperCase;

    virtual QStringList getList(GenericEditableTableModel *model) const;
    virtual QList<QPixmap> getIconList(GenericEditableTableModel *model) const;
    virtual QPixmap getColumnIcon(GenericEditableTableModel *model) const;

};

#endif // COMBOBOXDELEGATE_H
