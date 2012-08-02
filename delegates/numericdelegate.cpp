#include "numericdelegate.h"
#include <QtGui>

NumericDelegate::NumericDelegate(QObject *parent, bool allowFloatingPointNumbers) :
    QStyledItemDelegate(parent),
    allowFloatingPointNumbers(allowFloatingPointNumbers)
{
}

QWidget *NumericDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
    QLineEdit *editor=new QLineEdit(parent);;
    if(allowFloatingPointNumbers){
        editor->setValidator(new QDoubleValidator(editor));
    }else{
        editor->setValidator(new QIntValidator(editor));
    }
    editor->setFrame(false);

    return editor;
}

void NumericDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);

}

void NumericDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
     model->setData(index, lineEdit->text(), Qt::EditRole);
}

void NumericDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     editor->setGeometry(option.rect);
}
