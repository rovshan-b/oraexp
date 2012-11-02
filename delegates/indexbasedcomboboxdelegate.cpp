#include "indexbasedcomboboxdelegate.h"
#include <QtGui>

IndexBasedComboBoxDelegate::IndexBasedComboBoxDelegate(const QIcon &itemIcon, const QStringList &itemList, QObject *parent) :
    ComboBoxDelegate(parent, false, itemIcon, itemList)
{
}

//when using this constructor you must override createEditor method
IndexBasedComboBoxDelegate::IndexBasedComboBoxDelegate(QObject *parent) :
    ComboBoxDelegate(parent, false)
{
}

void IndexBasedComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int currentIx = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(currentIx);
}

void IndexBasedComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QComboBox *comboBox = static_cast<QComboBox*>(editor);

     model->setData(index, comboBox->currentIndex(), Qt::EditRole);
     model->setData(index, comboBox->currentText(), Qt::DisplayRole);
}
