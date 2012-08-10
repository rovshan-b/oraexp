#include "infopanel.h"
#include "util/widgethelper.h"
#include <QtGui>

InfoPanel::InfoPanel(QWidget *parent) :
    QWidget(parent)
{
    tab=new QStackedWidget();
    WidgetHelper::changeFontSize(tab, -2);
    createToolbar();

    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->addWidget(tab);
    mainLayout->addWidget(toolbar);
    mainLayout->setAlignment(toolbar, Qt::AlignBottom);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void InfoPanel::createToolbar()
{
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(12,12));
    WidgetHelper::changeFontSize(toolbar, -2);

    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setContentsMargins(0,0,0,0);
}

void InfoPanel::addPanel(QWidget *panel, const QString &title, const QIcon &icon)
{
    QAction *action=toolbar->addAction(icon, title);
    action->setData(tab->count());
    action->setCheckable(true);
    WidgetHelper::changeFontSize(action, -2);

    tab->addWidget(panel);

    //triggered signal is not emitted when changing status programmatically
    connect(action, SIGNAL(triggered(bool)), this, SLOT(buttonToggled(bool)));
}

void InfoPanel::setCurrentIndex(int index)
{
    Q_ASSERT(index>=0 && index<tab->count());

    setUpdatesEnabled(false);

    if(!tab->isVisible()){
        tab->show();
    }

    tab->setCurrentIndex(index);
    QList<QAction*> toolbarActions=toolbar->actions();
    QAction *action=toolbarActions.at(index);
    if(!action->isChecked()){
        action->setChecked(true);
    }

    for(int i=0; i<toolbarActions.size(); ++i){
        if(i!=index){
            toolbarActions.at(i)->setChecked(false);
        }
    }

    setUpdatesEnabled(true);
}

void InfoPanel::buttonToggled(bool checked)
{
    if(!checked){
        tab->setVisible(false);
        return;
    }

    QAction *action = static_cast<QAction*>(sender());
    Q_ASSERT(action);

    int actionIx = action->data().toInt();
    setCurrentIndex(actionIx);
}
