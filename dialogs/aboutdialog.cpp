#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "creditsdialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->closeButton->setDefault(true);
    ui->closeButton->setAutoDefault(true);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_infoLabel_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutDialog::on_closeButton_clicked()
{
    accept();
}

void AboutDialog::on_creditsButton_clicked()
{
    CreditsDialog dialog(this);
    dialog.exec();
}

void AboutDialog::on_websiteLabel_linkActivated(const QString &link)
{
    on_infoLabel_linkActivated(link);
}
