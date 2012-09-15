#ifndef COMPILERMESSAGESPANE_H
#define COMPILERMESSAGESPANE_H

#include <QWidget>
#include <QModelIndex>

class DataTable;
class QStandardItemModel;

class CompilerMessagesPane : public QWidget
{
    Q_OBJECT
public:
    explicit CompilerMessagesPane(QWidget *parent = 0);

    DataTable *table;

    void addCompilerMessage(int line, int position, const QString &message, const QString &attribute);
    void clearCompilerMessages();
    bool isEmpty() const;
    void resizeToFit();

signals:
    void activated(int line, int position, const QString &message);

private slots:
    void tableRowActivated(const QModelIndex &index);
    
private:
    QStandardItemModel *tableModel;
    
};

#endif // COMPILERMESSAGESPANE_H
