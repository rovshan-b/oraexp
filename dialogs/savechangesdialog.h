#ifndef SAVECHANGESDIALOG_H
#define SAVECHANGESDIALOG_H

#include <QDialog>
#include <QIcon>

class ConnectionPage;
class ConnectionPageTab;
class QTreeView;
class CheckableStandardItemModel;
class QStandardItem;

class SaveChangesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveChangesDialog(bool showDiscardButton, QWidget *parent = 0);

    void addConnection(ConnectionPage *cnPage, const QIcon &icon, const QString &title);
    void addTab(ConnectionPageTab *tab, const QIcon &icon, const QString &title);
    bool isEmpty() const;
    void setReady();

private slots:
    void save();
    void discard();
    
private:
    QTreeView *tree;
    CheckableStandardItemModel *model;
    QStandardItem *currentParentItem;
    ConnectionPage *currentCnPage;

    void addRow(ConnectionPageTab *tab, const QIcon &icon, const QString &title);
    
};

#endif // SAVECHANGESDIALOG_H
