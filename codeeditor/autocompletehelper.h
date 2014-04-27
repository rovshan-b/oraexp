#ifndef AUTOCOMPLETEHELPER_H
#define AUTOCOMPLETEHELPER_H

#include <QObject>

class CodeEditor;
class IQueryScheduler;
class QAbstractItemModel;

class AutocompleteHelper : public QObject
{
    Q_OBJECT
public:
    explicit AutocompleteHelper(QObject *parent);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    bool isBusy() const {return this->busy;}

    void getChildList(const QStringList &objectNameParts, int cursorPosition);

signals:
    void modelReady(QAbstractItemModel *model, int cursorPosition);

private:
    IQueryScheduler *queryScheduler;

    bool busy;
    
};

#endif // AUTOCOMPLETEHELPER_H
