#include "specbodyswitcherwidget.h"
#include <QtGui>

SpecBodySwitcherWidget::SpecBodySwitcherWidget(bool isSpec, QWidget *parent) :
    QWidget(parent),
    isSpec(isSpec)
{
    QHBoxLayout *layout=new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QToolBar *toolbar=new QToolBar();

    QAction *specButton = toolbar->addAction(tr("Spec"));
    specButton->setCheckable(true);
    QAction *bodyButton = toolbar->addAction(tr("Body"));
    bodyButton->setCheckable(true);

    if(isSpec){
        specButton->setChecked(true);
        bodyButton->setShortcut(QKeySequence(tr("F8","CodeCreator|Toggle Spec/Body")));
    }else{
        bodyButton->setChecked(true);
        specButton->setShortcut(QKeySequence(tr("F8","CodeCreator|Toggle Spec/Body")));
    }

    specBodySwitcherGroup=new QActionGroup(this);
    specBodySwitcherGroup->addAction(specButton);
    specBodySwitcherGroup->addAction(bodyButton);

    layout->addWidget(toolbar);

    setLayout(layout);

    connect(specBodySwitcherGroup, SIGNAL(triggered(QAction*)), this, SLOT(specBodySwitcherClicked(QAction*)));
}

void SpecBodySwitcherWidget::specBodySwitcherClicked(QAction *)
{
    if(isSpec){
        specBodySwitcherGroup->actions().at(0)->setChecked(true);
    }else{
        specBodySwitcherGroup->actions().at(1)->setChecked(true);
    }

    emit specBodySwitchRequested();
}
