#include "contrainttypedelegate.h"
#include "util/iconutil.h"
#include <QtGui>

ContraintTypeDelegate::ContraintTypeDelegate(QObject *parent) :
    IndexBasedComboBoxDelegate(parent)
{
}

QWidget *ContraintTypeDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index) const
{
     QComboBox *editor=(QComboBox*)IndexBasedComboBoxDelegate::createEditor(parent, option, index);

     editor->addItem(IconUtil::getIcon("column_pk"), tr("Primary key"));
     editor->addItem(IconUtil::getIcon("column_fk"), tr("Foreign key"));
     editor->addItem(IconUtil::getIcon("column_uq"), tr("Unique"));

     editor->setCurrentIndex(0);

     return editor;
}
