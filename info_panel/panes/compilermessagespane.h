#ifndef COMPILERMESSAGESPANE_H
#define COMPILERMESSAGESPANE_H

#include "info_panel/infopane.h"

class DataTable;
class QStandardItemModel;

class CompilerMessagesPane : public InfoPane
{
    Q_OBJECT
public:
    explicit CompilerMessagesPane(QWidget *parent = 0);

    DataTable *table;

    void addCompilerMessage(int line, int position, const QString &message, const QString &attribute);
    void clearCompilerMessages();
    bool isEmpty() const;
    void resizeToFit();
    
private:
    QStandardItemModel *tableModel;
    
};

#endif // COMPILERMESSAGESPANE_H
