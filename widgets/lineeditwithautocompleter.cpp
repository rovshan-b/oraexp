#include "lineeditwithautocompleter.h"
#include "interfaces/istringlistretriever.h"
#include "util/widgethelper.h"
#include <QtGui>

LineEditWithAutocompleter::LineEditWithAutocompleter(QWidget *parent) :
    QLineEdit(parent), itemListRetriever(0)
{
}

void LineEditWithAutocompleter::setItemListRetriever(IStringListRetriever *itemListRetriever)
{
    this->itemListRetriever=itemListRetriever;
}

IStringListRetriever *LineEditWithAutocompleter::getItemListRetriever() const
{
    return this->itemListRetriever;
}

void LineEditWithAutocompleter::setItemList(const QStringList &itemList)
{
    this->itemList=itemList;
}

void LineEditWithAutocompleter::keyReleaseEvent(QKeyEvent * event)
{
    QLineEdit::keyReleaseEvent(event);
    //event->accept();
    if(itemListRetriever!=0 || !itemList.isEmpty()){
        int key=event->key();
        if(key!=Qt::Key_Delete && key!=Qt::Key_Backspace && key!=Qt::Key_Tab
                && key!=Qt::Key_Left && key!=Qt::Key_Right
                && key!=Qt::Key_Up && key!=Qt::Key_Down){
            WidgetHelper::completeLineEditText(this, itemListRetriever==0 ? itemList : itemListRetriever->getStringList());
        }
    }
}
