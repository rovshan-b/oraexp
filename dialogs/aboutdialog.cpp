#include "aboutdialog.h"
#include "util/iconutil.h"
#include "creditsdialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("All praise belongs to Allah!"));

    setWindowIcon(IconUtil::getIcon("about"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    createTopPane(mainLayout);
    createMiddlePane(mainLayout);
    createBottomPane(mainLayout);

    setLayout(mainLayout);
}

void AboutDialog::linkClicked(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void AboutDialog::showCreditsDialog()
{
    CreditsDialog dialog(this);
    dialog.exec();
}

void AboutDialog::createTopPane(QBoxLayout *layout)
{
    QVBoxLayout *rightLayout = new QVBoxLayout();

    QLabel *titleLabel = new QLabel("OraExp");
    QFont font = titleLabel->font();
    font.setPointSize(22);
    titleLabel->setFont(font);
    rightLayout->addWidget(titleLabel);

    rightLayout->addWidget(new QLabel("   Version: 0.1"));

    QLabel *websiteLabel = new QLabel("&nbsp;&nbsp;&nbsp;Website: <a href='http://www.oraexp.com'>www.oraexp.com</a>");
    connect(websiteLabel, SIGNAL(linkActivated(QString)), this, SLOT(linkClicked(QString)));
    rightLayout->addWidget(websiteLabel);

    rightLayout->addWidget(new QLabel("   Author: Rovshan Baghirov"));


    QHBoxLayout *fullLayout = new QHBoxLayout();
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(IconUtil::getIcon("database_big"));
    fullLayout->addWidget(logoLabel, 0, Qt::AlignLeft|Qt::AlignTop);

    fullLayout->addLayout(rightLayout);

    layout->addLayout(fullLayout);
}

void AboutDialog::createMiddlePane(QBoxLayout *layout)
{
    QLabel *infoLabel = new QLabel("Please, send all your comments to <a href='mailto:feedback@oraexp.com?subject=OraExp - feedback'>feedback@oraexp.com</a>. "
                                   "Any positive or negative feedback is highly appreciated");
    infoLabel->setWordWrap(true);
    connect(infoLabel, SIGNAL(linkActivated(QString)), this, SLOT(linkClicked(QString)));
    layout->addWidget(infoLabel);
}

void AboutDialog::createBottomPane(QBoxLayout *layout)
{
    QPushButton *creditsButton = new QPushButton(IconUtil::getIcon("about"), tr("C&redits"));
    connect(creditsButton, SIGNAL(clicked()), this, SLOT(showCreditsDialog()));

    QPushButton *closeButton = new QPushButton(IconUtil::getIcon("yes"), tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    QHBoxLayout *fullLayout = new QHBoxLayout();
    fullLayout->addWidget(creditsButton);
    fullLayout->addStretch();
    fullLayout->addWidget(closeButton);

    layout->addLayout(fullLayout);
}
