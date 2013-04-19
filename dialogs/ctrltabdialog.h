#ifndef CTRLTABDIALOG_H
#define CTRLTABDIALOG_H

#include <QDialog>
#include <QModelIndex>

class QListView;
class CtrlTabData;
class QStandardItemModel;

class CtrlTabDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CtrlTabDialog(const QList<CtrlTabData*> &data, QWidget *parent);

    QWidget* getSelectedWidget() const;
    
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );

private slots:
    void listItemActivated(const QModelIndex &index);

private:
    QListView *listView;
    QStandardItemModel *listModel;

    void populateList(const QList<CtrlTabData*> &data);

    bool handleKeyEvent(QKeyEvent *keyEvent, bool keyPress);

    void move(bool next);
    
};

#endif // CTRLTABDIALOG_H
