#include "creditsdialog.h"
#include "ui_creditsdialog.h"
#include <QtGui>

CreditsDialog::CreditsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreditsDialog)
{
    ui->setupUi(this);

    connect(ui->crLabel1, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
    connect(ui->crLabel2, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
    connect(ui->crLabel3, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
}

CreditsDialog::~CreditsDialog()
{
    delete ui;
}

void CreditsDialog::openUrl(const QString &link)
{
    QDesktopServices::openUrl(link);
}
