#include "ondeleteactiondelegate.h"
#include <QtGui>

OnDeleteActionDelegate::OnDeleteActionDelegate(QObject *parent) :
    IndexBasedComboBoxDelegate(parent)
{
}

QWidget *OnDeleteActionDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index) const
{
     QComboBox *editor=(QComboBox*)ComboBoxDelegate::createEditor(parent, option, index);

     editor->addItem(tr("No action"));
     editor->addItem(tr("Cascade"));
     editor->addItem(tr("Set null"));

     editor->setCurrentIndex(0);

     return editor;
}

