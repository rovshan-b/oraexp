#include "indexbasedcomboboxdelegate.h"
#include <QtGui>
#include <QDebug>

IndexBasedComboBoxDelegate::IndexBasedComboBoxDelegate(QObject *parent, int modelColumnIx) :
    ComboBoxDelegate(parent, modelColumnIx, false)
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

     int currentIndex=comboBox->currentIndex();
     QString currentText=comboBox->currentText();

     model->setData(index, currentIndex, Qt::EditRole);
     model->setData(index, currentText, Qt::DisplayRole);
     model->setData(index, currentText.isEmpty() ? QVariant() : comboBox->itemIcon(currentIndex), Qt::DecorationRole);
}
