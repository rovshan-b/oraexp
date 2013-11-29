#include "tabbar.h"
#include "util/iconutil.h"
#include <QtGui>

TabBar::TabBar(QWidget *parent) :
    QTabBar(parent),
    addButtonVisible(true)
{
    addButton = new QToolButton();
    addButton->setToolTip(tr("New worksheet"));
    addButton->setAutoRaise(true);
    addButton->setIcon(IconUtil::getIcon("add_small"));
    addButton->setParent(this);

    movePlusButton();
}

QSize TabBar::sizeHint() const
{
    QSize hint = QTabBar::sizeHint();
    return QSize(hint.width() + 25, hint.height());
}

void TabBar::resizeEvent(QResizeEvent *event)
{
    QTabBar::resizeEvent(event);

    movePlusButton();
}

void TabBar::tabLayoutChange()
{
    QTabBar::tabLayoutChange();

    movePlusButton();
}

void TabBar::movePlusButton()
{
    int size = 0;
    for(int i=0; i<count(); ++i){
        size += tabRect(i).width();
    }

    addButton->resize(25, tabRect(count()-1).height());

    int top = geometry().top();
    int w = width();

    if(size > w){
        //emit setCornerWidget(addButton);
        addButtonVisible = false;
    }else{
        if(!addButtonVisible){
            //emit setCornerWidget(0);
            addButtonVisible = true;
        }
        addButton->move(size, top);
    }
}


