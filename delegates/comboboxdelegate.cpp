#include "comboboxdelegate.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent, int modelColumnIx, bool isEditable, bool convertToUpperCase) :
    QStyledItemDelegate(parent), modelColumnIx(modelColumnIx), isEditable(isEditable), convertToUpperCase(convertToUpperCase)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &index) const
{
     QComboBox *editor=new QComboBox(parent);;

     editor->setEditable(isEditable);

     GenericEditableTableModel *model = static_cast<GenericEditableTableModel*>(const_cast<QAbstractItemModel*>(index.model()));
     QStringList items=getList(model);
     QList<QPixmap> icons=getIconList(model);

     Q_ASSERT(icons.size()==0 || icons.size()==items.size());

     int itemCount=items.size();
     bool hasIcons = icons.size()>0;
     QPixmap colIcon = hasIcons ? QPixmap() : getColumnIcon(model);

     QString currentItem;
     for(int i=0; i<itemCount; ++i){
         currentItem=items.at(i);
         if(currentItem.isEmpty()){
            editor->addItem(currentItem);
         }else{
             editor->addItem(hasIcons ? icons.at(i) : colIcon, currentItem);
         }
     }

     editor->setCurrentIndex(0);

     if(isEditable){
        editor->setFrame(false);
     }

     return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if(!value.isEmpty()){

        int editorItemIndex=comboBox->findText(value, Qt::MatchFixedString);
        if(editorItemIndex!=-1){
            comboBox->setCurrentIndex(editorItemIndex);
        }else if(editorItemIndex==-1 && isEditable){
            GenericEditableTableModel *model = static_cast<GenericEditableTableModel*>(const_cast<QAbstractItemModel*>(index.model()));
            QPixmap itemIcon = model->getColumnIcon(modelColumnIx);

            comboBox->insertItem(0, itemIcon, "");
            comboBox->insertItem(1, itemIcon, value);
            comboBox->setCurrentIndex(1);
        }

        //else{
        //    if(isEditable){
        //        comboBox->insertItem(0, itemIcon, value);
        //        comboBox->setCurrentIndex(0);
        //    }
        //}
    }else if(isEditable && value.isEmpty()){
        comboBox->insertItem(0, "");
        comboBox->setCurrentIndex(0);
    }else if(!isEditable && comboBox->currentIndex()==-1){
        comboBox->setCurrentIndex(0);
    }

    if(isEditable){
        comboBox->lineEdit()->selectAll();
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
     QComboBox *comboBox = static_cast<QComboBox*>(editor);

     QString value = comboBox->currentText().trimmed();
     if(convertToUpperCase){
         value=value.toUpper();
     }
     int selectedIndex = comboBox->currentIndex();

     if(model->data(index, Qt::EditRole).toString() == value &&
             model->data(index, Qt::UserRole+1).toInt() == selectedIndex){
         return;
     }

     model->setData(index, value.isEmpty() ? QIcon() : comboBox->itemIcon(selectedIndex), Qt::DecorationRole);

     model->setData(index, value, Qt::EditRole);
     model->setData(index, selectedIndex, Qt::UserRole+1);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     editor->setGeometry(option.rect);
}

QStringList ComboBoxDelegate::getList(GenericEditableTableModel *model) const
{
    Q_ASSERT(modelColumnIx!=-1);

    return model->getList(modelColumnIx);
}

QList<QPixmap> ComboBoxDelegate::getIconList(GenericEditableTableModel *model) const
{
    Q_ASSERT(modelColumnIx!=-1);

    return model->getIconList(modelColumnIx);
}

QPixmap ComboBoxDelegate::getColumnIcon(GenericEditableTableModel *model) const
{
    Q_ASSERT(modelColumnIx!=-1);

    return model->getColumnIcon(modelColumnIx);
}
