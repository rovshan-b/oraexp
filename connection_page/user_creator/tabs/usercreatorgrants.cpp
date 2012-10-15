#include "usercreatorgrants.h"
#include "util/widgethelper.h"
#include "usercreatorgrantssimplelayout.h"
#include "usercreatorgrantsadvancedlayout.h"
#include <QtGui>

UserCreatorGrants::UserCreatorGrants(UserCreatorTabs *userCreator, bool editMode, QWidget *parent) :
    UserCreatorTab(userCreator, editMode, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QHBoxLayout *modeSelectionLayout=new QHBoxLayout();
    simpleModeRadio=new QRadioButton(tr("&Simple mode"));
    simpleModeRadio->setChecked(true);
    modeSelectionLayout->addWidget(simpleModeRadio);
    advancedModeRadio=new QRadioButton(tr("&Advanced mode"));
    modeSelectionLayout->addWidget(advancedModeRadio);

    mainLayout->addLayout(modeSelectionLayout);
    mainLayout->setAlignment(modeSelectionLayout, Qt::AlignTop|Qt::AlignLeft);

    tab = new QStackedWidget();

    createSimpleLayout();
    createAdvancedLayout();

    mainLayout->addWidget(tab);
    setLayout(mainLayout);

    connect(simpleModeRadio, SIGNAL(toggled(bool)), this, SLOT(switchMode(bool)));
}

void UserCreatorGrants::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    UserCreatorTab::setQueryScheduler(queryScheduler);

    advancedLayout->setQueryScheduler(queryScheduler);
}

void UserCreatorGrants::switchMode(bool simpleMode)
{
    tab->setCurrentIndex(simpleMode ? 0 : 1);
}

void UserCreatorGrants::createSimpleLayout()
{
    simpleLayout = new UserCreatorGrantsSimpleLayout();
    tab->addWidget(simpleLayout);
}

void UserCreatorGrants::createAdvancedLayout()
{
    advancedLayout = new UserCreatorGrantsAdvancedLayout();
    tab->addWidget(advancedLayout);
}
