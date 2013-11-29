#include "optionsdialogconnectivitytab.h"
#include "util/settingshelper.h"
#include <QtGui>

OptionsDialogConnectivityTab::OptionsDialogConnectivityTab(QWidget *parent) :
    OptionsDialogTab(parent)
{
}


void OptionsDialogConnectivityTab::doCreateUi()
{
    QVBoxLayout *topLayout = new QVBoxLayout();

    useSeparateSessionsCheckBox = new QCheckBox(tr("Use separate session for each tab"));
    topLayout->addWidget(useSeparateSessionsCheckBox);

    addToMainLayout(topLayout);

    useSeparateSessionsCheckBox->setChecked(SettingsHelper::getUseSeparateSessions());
}

void OptionsDialogConnectivityTab::saveSettings()
{
    SettingsHelper::setUseSeparateSessions(useSeparateSessionsCheckBox->isChecked());
}
