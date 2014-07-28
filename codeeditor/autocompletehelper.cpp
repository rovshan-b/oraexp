#include "autocompletehelper.h"
#include "codeeditor.h"

#include <QDebug>

AutocompleteHelper::AutocompleteHelper(QObject *parent) :
    QObject(parent),
    busy(false)
{

}

void AutocompleteHelper::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}

void AutocompleteHelper::getChildList(const TokenNameInfo &tokenNameInfo)
{
    if(this->busy){
        return;
    }

    qDebug() << "get child list for" << tokenNameInfo.toString();
}
