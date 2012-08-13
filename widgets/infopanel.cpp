#include "infopanel.h"
#include "util/widgethelper.h"
#include "widgets/closebutton.h"
#include <QtGui>

InfoPanel::InfoPanel(QWidget *parent) :
    QObject(parent)
{
    tab=new QStackedWidget();
    WidgetHelper::changeFontSize(tab, -2);
    tab->hide();

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
    action->setChecked(false);
    WidgetHelper::changeFontSize(action, -2);

    QWidget *containerWidget=new QWidget();
    QVBoxLayout *containerLayout=new QVBoxLayout();
    containerLayout->setContentsMargins(0,0,0,0);
    containerLayout->setSpacing(0);

    containerLayout->addLayout(createPanelHeader(title));
    containerLayout->addWidget(panel);

    containerWidget->setLayout(containerLayout);
    tab->addWidget(containerWidget);

    //triggered signal is not emitted when changing status programmatically
    connect(action, SIGNAL(triggered(bool)), this, SLOT(buttonToggled(bool)));
}

void InfoPanel::setCurrentIndex(int index)
{
    Q_ASSERT(index>=0 && index<tab->count());

    toolbar->setUpdatesEnabled(false);

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

    toolbar->setUpdatesEnabled(true);
}

int InfoPanel::getCurrentIndex() const
{
    return tab->isVisible() ? tab->currentIndex() : -1;
}

bool InfoPanel::isPanelVisible() const
{
    return tab->isVisible();
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

void InfoPanel::closePanel()
{
    int currentIx = getCurrentIndex();
    if(currentIx!=-1){
        toolbar->actions().at(currentIx)->toggle();
        tab->hide();
    }
}

QBoxLayout *InfoPanel::createPanelHeader(const QString &title)
{
    QHBoxLayout *headerLayout=new QHBoxLayout();
    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->addWidget(new QLabel(title));

    headerLayout->addStretch();

    CloseButton *closeButton = new CloseButton();
    headerLayout->addWidget(closeButton);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(closePanel()));

    return headerLayout;
}
