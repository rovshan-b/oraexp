#ifndef SAVECHANGESDIALOG_H
#define SAVECHANGESDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QItemSelection>

class ConnectionPage;
class ConnectionPageTab;
class QTreeView;
class CheckableStandardItemModel;
class SavePreviewWidget;
class QDialogButtonBox;
class QStandardItem;
class QBoxLayout;

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
    
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
private:
    QTreeView *tree;
    CheckableStandardItemModel *model;
    SavePreviewWidget *previewWidget;
    QDialogButtonBox *buttonBox;
    QStandardItem *currentParentItem;
    ConnectionPage *currentCnPage;

    void createTreeAndPreviewPane(QBoxLayout *mainLayout);
    void createButtonBox(QBoxLayout *mainLayout, bool showDiscardButton);
    void addRow(ConnectionPageTab *tab, const QIcon &icon, const QString &title);
    
};

#endif // SAVECHANGESDIALOG_H
