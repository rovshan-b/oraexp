#ifndef AUTOCOMPLETEHELPER_H
#define AUTOCOMPLETEHELPER_H

#include <QObject>
#include "beans/tokennameinfo.h"

class CodeEditor;
class IQueryScheduler;
class QAbstractItemModel;
class MultiEditorWidget;

class AutocompleteHelper : public QObject
{
    Q_OBJECT
public:
    explicit AutocompleteHelper(QObject *parent);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    bool isBusy() const {return this->busy;}

    void prepareCompletionList(MultiEditorWidget *multiEditor);

signals:
    void modelReady(QAbstractItemModel *model, int cursorPosition);

private:
    IQueryScheduler *queryScheduler;

    void getChildList(const TokenNameInfo &tokenNameInfo);

    bool busy;
    
};

#endif // AUTOCOMPLETEHELPER_H
