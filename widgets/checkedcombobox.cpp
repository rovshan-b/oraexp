#include "checkedcombobox.h"
#include "delegates/checkboxlistdelegate.h"
#include <QtGui>

CheckedComboBox::CheckedComboBox(QWidget *parent) :
    QComboBox(parent)
{
    CheckBoxListDelegate *checkBoxListDelegate=new CheckBoxListDelegate(this);
    view()->setItemDelegate(checkBoxListDelegate);
    view()->setEditTriggers(QAbstractItemView::CurrentChanged);
    view()->viewport()->installEventFilter(this);

    setEditable(true);
}


bool CheckedComboBox::eventFilter(QObject *object, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonRelease &&
            object==view()->viewport()){
        return true;
    }

    return QComboBox::eventFilter(object, event);
}

void CheckedComboBox::showPopup ()
{
    QStringList parts=lineEdit()->text().split(',');
    int partCount=parts.count();
    QAbstractItemModel *currentModel=model();
    int itemCount=currentModel->rowCount();
    QString part;
    QModelIndex index;
    for(int i=0; i<partCount; ++i){
        part=parts.at(i).trimmed();
        for(int k=0; k<itemCount; ++k){
            index=currentModel->index(k, 0);
            if(index.data().toString()==part){
                currentModel->setData(index,
                                  true,
                                  Qt::UserRole);
            }
        }
    }

    QComboBox::showPopup();
}

void CheckedComboBox::hidePopup ()
{
    int itemCount=count();
    QModelIndex index;
    QAbstractItemModel *currentModel=model();
    bool isItemChecked;

    QString resultText;
    for(int i=0; i<itemCount; ++i){
        index=currentModel->index(i, 0);
        isItemChecked=currentModel->data(index, Qt::UserRole).toBool();
        if(isItemChecked){
            QString itemText=index.data().toString();
            resultText.append(itemText).append(", ");
        }
    }

    if(!resultText.isEmpty()){
        resultText.remove(resultText.size()-2, 2);
    }

    if(lineEdit()->text()!=resultText){
        setEditText(resultText);
    }

    QComboBox::hidePopup();
}
