#include "queryrunnerdialog.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include "connectivity/dbconnection.h"
#include <QDebug>
#include <QtGui>

QueryRunnerDialog::QueryRunnerDialog(const QString &query, const QList<Param *> &params, DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    query(query),
    params(params),
    progressBar(0),
    cancelButton(0),
    cancelling(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    messageLabel = new QLabel(tr("Working..."));
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    mainLayout->addWidget(messageLabel, 0, Qt::AlignHCenter);

    setLayout(mainLayout);

    QSize hint = sizeHint();
    resize(qMax(hint.width(), 150), hint.height());
    DialogHelper::centerWindow(this);

    QTimer::singleShot(1000, this, SLOT(makeVisible()));
    QTimer::singleShot(3000, this, SLOT(addProgressControls()));
}

void QueryRunnerDialog::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);

    setInProgress(true);

    qDebug() << "query =" << query;

    enqueueQuery(QString("$%1").arg(query), params, this, "query_runner_query",
                 "queryCompleted", "recordAvailable", "fetchCompleted");

    if(cancelButton){
        cancelButton->setVisible(true);
    }
}

void QueryRunnerDialog::queryCompleted(const QueryResult &result)
{
    hideProgressBar();

    if(result.hasError){
        setInProgress(false);
        if(cancelling){
            accept();
        }else{
            messageLabel->setText(result.exception.getErrorMessage());
        }
    }
}

void QueryRunnerDialog::recordAvailable(const FetchResult &result)
{
    if(result.hasError){
        messageLabel->setText(result.exception.getErrorMessage());
        return;
    }

    messageLabel->setText(result.oneRow.at(0));
}

void QueryRunnerDialog::fetchCompleted(const QString &)
{
    setInProgress(false);

    makeVisible();
}

void QueryRunnerDialog::addProgressControls()
{
    QBoxLayout *mainLayout = static_cast<QBoxLayout*>(layout());

    progressBar = new QProgressBar();
    progressBar->setMaximumHeight(10);
    progressBar->setRange(0,0);
    progressBar->setVisible(isInProgress());
    mainLayout->addWidget(progressBar);

    if(this->requiresSeparateConnection){
        cancelButton = new QPushButton(IconUtil::getIcon("stop"), tr("Cancel"));
        cancelButton->setVisible(isInProgress());
        mainLayout->addWidget(cancelButton);
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    }
}

void QueryRunnerDialog::cancel()
{
    hideProgressBar();
    cancelling = true;
    //messageLabel->setText(tr("Cancelling..."));

    getDb()->breakCurrentQuery();
}

void QueryRunnerDialog::hideProgressBar()
{
    if(progressBar){
        progressBar->hide();
    }
    if(cancelButton){
        cancelButton->hide();
    }
}
