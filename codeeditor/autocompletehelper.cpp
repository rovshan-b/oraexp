#include "autocompletehelper.h"
#include "codeeditor.h"

AutocompleteHelper::AutocompleteHelper(QObject *parent) :
    QObject(parent)
{

}

void AutocompleteHelper::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}
