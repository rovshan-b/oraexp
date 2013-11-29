#ifndef OBJECTLOOKUPDIALOG_H
#define OBJECTLOOKUPDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include <QModelIndex>

class QLineEdit;
class DataTable;

class ObjectLookupDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    explicit ObjectLookupDialog(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createUi();

protected:
    virtual void closeEvent ( QCloseEvent * event );
    void focusOutEvent ( QFocusEvent * event );
    void showEvent ( QShowEvent * event );
    void hideEvent ( QHideEvent * event );

    void timerEvent ( QTimerEvent * event );

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void textEdited(const QString &newText);
    void firstFetchCompleted();

    void focusSearchBox();

    void indexActivated( const QModelIndex & index );
    
private:
    QLineEdit *objectNameEditor;
    DataTable *table;
    QString lastText;

    int timerId;

    bool searchInProgress;

    void stopTimer();
    
};

#endif // OBJECTLOOKUPDIALOG_H
