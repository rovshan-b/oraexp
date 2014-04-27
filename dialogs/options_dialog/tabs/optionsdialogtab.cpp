#include "optionsdialogtab.h"
#include <QtGui>

OptionsDialogTab::OptionsDialogTab(QWidget *parent) :
    QWidget(parent)
{
}

void OptionsDialogTab::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    doCreateUi();
}

bool OptionsDialogTab::isInitialized() const
{
    return layout() != 0;
}

void OptionsDialogTab::addToMainLayout(QLayout *childLayout)
{
    QBoxLayout *mainLayout = static_cast<QBoxLayout*>(layout());
    mainLayout->addLayout(childLayout);
    mainLayout->setAlignment(childLayout, Qt::AlignLeft|Qt::AlignTop);
}
