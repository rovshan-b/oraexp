#include "autocompletehelper.h"
#include "codeeditor.h"

AutocompleteHelper::AutocompleteHelper(QObject *parent) :
    QObject(parent)
{
    editor = qobject_cast<CodeEditor*>(parent);
    Q_ASSERT(editor);
}

void AutocompleteHelper::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}
