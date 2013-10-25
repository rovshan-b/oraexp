#include "optionsdialog.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "util/settingshelper.h"
#include <QtGui>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Options"));
    setWindowIcon(IconUtil::getIcon("settings"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *centerLayout = new QHBoxLayout();

    createLeftPane(centerLayout);
    createRightPane(centerLayout);

    mainLayout->addLayout(centerLayout);

    QDialogButtonBox *buttonBox = DialogHelper::createButtonBox(this, QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    initializeValues();

    connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
}

void OptionsDialog::createLeftPane(QBoxLayout *layout)
{
    QVBoxLayout *leftLayout = new QVBoxLayout();

    sidePane = new QListWidget();
    sidePane->addItem(new QListWidgetItem(IconUtil::getIcon("connect"), tr("Connectivity")));
    sidePane->setCurrentRow(0);
    sidePane->setMaximumWidth(fontMetrics().width(sidePane->item(0)->text())+50);
    leftLayout->addWidget(sidePane);

    layout->addLayout(leftLayout);
    layout->setAlignment(leftLayout, Qt::AlignLeft | Qt::AlignTop);
}

void OptionsDialog::createRightPane(QBoxLayout *layout)
{
    QVBoxLayout *rightLayout = new QVBoxLayout();

    QLabel *titleLabel = new QLabel(tr("<h3>Connectivity</h3>"));
    rightLayout->addWidget(titleLabel);

    useSeparateSessionsCheckBox = new QCheckBox(tr("Use separate session for each tab"));
    rightLayout->addWidget(useSeparateSessionsCheckBox);

    layout->addLayout(rightLayout);
    layout->setAlignment(rightLayout, Qt::AlignLeft | Qt::AlignTop);
}

void OptionsDialog::accept()
{
    apply();

    QDialog::accept();
}

void OptionsDialog::apply()
{
    SettingsHelper::setUseSeparateSessions(useSeparateSessionsCheckBox->isChecked());
}


void OptionsDialog::initializeValues()
{
    useSeparateSessionsCheckBox->setChecked(SettingsHelper::getUseSeparateSessions());
}
