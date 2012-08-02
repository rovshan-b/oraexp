#include "abstractcomboboxdelegate.h"
#include <QtGui>

AbstractComboBoxDelegate::AbstractComboBoxDelegate(QObject *parent, bool isEditable, const QIcon &itemIcon, bool convertToUpperCase) :
    QStyledItemDelegate(parent), isEditable(isEditable), itemIcon(itemIcon), convertToUpperCase(convertToUpperCase)
{
}

QWidget *AbstractComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
     QComboBox *editor=new QComboBox(parent);;

     editor->setEditable(isEditable);

     QStringList items=getList();

     int itemCount=items.size();
     QString currentItem;
     for(int i=0; i<itemCount; ++i){
         currentItem=items.at(i);
         if(itemIcon.isNull() || currentItem.isEmpty()){
            editor->addItem(currentItem);
         }else{
            editor->addItem(itemIcon, currentItem);
         }
     }

     editor->setCurrentIndex(0);

     if(isEditable){
        editor->setFrame(false);
     }

     return editor;
}

void AbstractComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if(!value.isEmpty()){

        if(isEditable){
            comboBox->setEditText(value);
        }

        int editorItemIndex=comboBox->findText(value, Qt::MatchFixedString);
        if(editorItemIndex!=-1){
            comboBox->setCurrentIndex(editorItemIndex);
        }

        //else{
        //    if(isEditable){
        //        comboBox->insertItem(0, itemIcon, value);
        //        comboBox->setCurrentIndex(0);
        //    }
        //}
    }else if(comboBox->currentIndex()==-1){
        comboBox->setCurrentIndex(0);
    }

    if(isEditable){
        comboBox->lineEdit()->selectAll();
    }
}

void AbstractComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QComboBox *comboBox = static_cast<QComboBox*>(editor);

     QString value = comboBox->currentText();
     if(convertToUpperCase){
         value=value.toUpper();
     }
     int selectedIndex = comboBox->currentIndex();

     model->setData(index, value.isEmpty() ? QIcon() : itemIcon, Qt::DecorationRole);

     model->setData(index, value, Qt::EditRole);
     model->setData(index, selectedIndex, Qt::UserRole+1);
}

void AbstractComboBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     editor->setGeometry(option.rect);
}
