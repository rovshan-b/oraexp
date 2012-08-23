#include "identifiernamedelegate.h"
#include "connectivity/dbconnection.h"
#include "defines.h"
#include <QtGui>

IdentifierNameDelegate::IdentifierNameDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *IdentifierNameDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
     QLineEdit *editor=new QLineEdit(parent);
     //editor->setContentsMargins(0, 0, 0, 0);
     editor->setFrame(false);

     editor->setMaxLength(MAX_IDENTIFIER_LENGTH);

     return editor;
}

void IdentifierNameDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void IdentifierNameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
     QString value=lineEdit->text().trimmed().toUpper();

     model->setData(index, value, Qt::EditRole);

     if(!value.isEmpty()){
         int modelRowCount=model->rowCount();
         if(modelRowCount-1==index.row()){
             model->insertRows(modelRowCount, 1);
         }
     }
}

void IdentifierNameDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     editor->setGeometry(option.rect);
}
