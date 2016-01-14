#ifndef AUTOCOMPLETEHELPER_H
#define AUTOCOMPLETEHELPER_H

#include <QObject>
#include "beans/tokennameinfo.h"
#include "connectivity/queryresult.h"

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

    void prepareCompletionList(MultiEditorWidget *multiEditor, bool terminatedBySeparator);

signals:
    void modelReady(QAbstractItemModel *model, int cursorPosition);

private slots:
    void childListReady(const QueryResult &result);
    void firstFetchCompleted();

private:
    IQueryScheduler *queryScheduler;

    void getChildList(const TokenNameInfo &tokenNameInfo, bool terminatedBySeparator);

    bool busy;
    
};

#endif // AUTOCOMPLETEHELPER_H
