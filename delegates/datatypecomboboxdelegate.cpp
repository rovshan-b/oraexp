#include "datatypecomboboxdelegate.h"
#include "defines.h"
#include <QtGui>

DataTypeComboBoxDelegate::DataTypeComboBoxDelegate(QObject *parent, int modelColumnIx) :
    ComboBoxDelegate(parent, modelColumnIx)
{
}

QWidget *DataTypeComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index) const
{
    currentModel=const_cast<QAbstractItemModel*>(index.model());

     QComboBox *editor=(QComboBox*)ComboBoxDelegate::createEditor(parent, option, index);

     QVariant originalValue=index.data(DATA_TYPE_ORIGINAL_VALUE_ROLE_ID);
     if(!originalValue.isNull()){
         editor->addItem(originalValue.toString());
     }

     editor->addItem("NUMBER");
     editor->addItem("DATE");
     editor->addItem("VARCHAR2(50)");
     editor->addItem("VARCHAR2(50 CHAR)");
     editor->addItem("NVARCHAR2(50)");
     editor->addItem("CHAR(10)");
     editor->addItem("CHAR(10 CHAR)");
     editor->addItem("NCHAR(10)");
     editor->addItem("CLOB");
     editor->addItem("NCLOB");
     editor->addItem("BLOB");
     editor->addItem("INTERVAL DAY TO SECOND");
     editor->addItem("INTERVAL YEAR TO MONTH");
     editor->addItem("TIMESTAMP");
     editor->addItem("TIMESTAMP WITH TIME ZONE");
     editor->addItem("TIMESTAMP WITH LOCAL TIME ZONE");

     return editor;
}

void DataTypeComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    ComboBoxDelegate::setEditorData(editor, index);

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();

    if(!value.isEmpty()){
        int editorItemIndex=comboBox->findText(value, Qt::MatchFixedString);
        if(editorItemIndex==-1){
            comboBox->insertItem(0, value);
            comboBox->setCurrentIndex(0);

            currentModel->setData(index, value, DATA_TYPE_ORIGINAL_VALUE_ROLE_ID);
        }
    }
}
