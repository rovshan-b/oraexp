#ifndef ABSTRACTCOMBOBOXDELEGATE_H
#define ABSTRACTCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class AbstractComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AbstractComboBoxDelegate(QObject *parent, bool isEditable=true, const QIcon &itemIcon=QIcon(), bool convertToUpperCase=true);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const;


protected:
    bool isEditable;
    QIcon itemIcon;
    bool convertToUpperCase;

    virtual QStringList getList() const=0;

};

#endif // ABSTRACTCOMBOBOXDELEGATE_H
