#include "radiobuttongroup.h"
#include <QtGui>

RadioButtonGroup::RadioButtonGroup(Qt::Orientation orientation, QWidget *parent) :
    QWidget(parent)
{
    if(orientation == Qt::Vertical){
        mainLayout = new QVBoxLayout();
    }else{
        mainLayout = new QHBoxLayout();
    }
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    group = new QButtonGroup(this);
}

void RadioButtonGroup::addRadioButton(QRadioButton *radio)
{
    mainLayout->addWidget(radio);
    group->addButton(radio, group->buttons().size());
}

int RadioButtonGroup::currentIndex() const
{
    return checkedId();
}

int RadioButtonGroup::checkedId() const
{
    return group->checkedId();
}

void RadioButtonGroup::checkRadio(int id)
{
    QAbstractButton *radio = group->button(id);
    if(radio){
        radio->setChecked(true);
    }
}
