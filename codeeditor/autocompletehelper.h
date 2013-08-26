#ifndef AUTOCOMPLETEHELPER_H
#define AUTOCOMPLETEHELPER_H

#include <QObject>

class CodeEditor;
class IQueryScheduler;

class AutocompleteHelper : public QObject
{
    Q_OBJECT
public:
    explicit AutocompleteHelper(QObject *parent);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

protected:
    IQueryScheduler *queryScheduler;
    
};

#endif // AUTOCOMPLETEHELPER_H
