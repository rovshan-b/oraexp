#ifndef QUERYRUNNERDIALOG_H
#define QUERYRUNNERDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class QLabel;
class QProgressBar;

class QueryRunnerDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    explicit QueryRunnerDialog(const QString &query,
                               const QList<Param *> &params,
                               DbUiManager *uiManager,
                               QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

private slots:
    void queryCompleted(const QueryResult &result);
    void recordAvailable(const FetchResult &result);
    void fetchCompleted(const QString &);

    void addProgressControls();

    void cancel();
    
private:
    QString query;
    QList<Param *> params;
    QLabel *messageLabel;
    QProgressBar *progressBar;
    QPushButton *cancelButton;

    bool cancelling;

    void hideProgressBar();
    
};

#endif // QUERYRUNNERDIALOG_H
