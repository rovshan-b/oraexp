#include "infopanel.h"
#include "infopane.h"
#include "util/widgethelper.h"
#include "widgets/closebutton.h"
#include <QtGui>

InfoPanel::InfoPanel(QWidget *parent) :
    QObject(parent)
{
    tab=new QStackedWidget();
    WidgetHelper::changeFontSize(tab, -1);
    tab->hide();

    createToolbar();
}

void InfoPanel::addPane(InfoPane *pane, const QString &title, const QIcon &icon)
{
    panes.append(pane);

    QAction *action=new QAction(icon, title, this);
    toolbar->insertAction(placeholderAction, action);
    action->setData(tab->count());
    action->setCheckable(true);
    action->setChecked(false);
    WidgetHelper::changeFontSize(action, -1);

    QWidget *containerWidget=new QWidget();
    QVBoxLayout *containerLayout=new QVBoxLayout();
    containerLayout->setContentsMargins(0,0,0,0);
    containerLayout->setSpacing(0);

    containerLayout->addLayout(createPanelHeader(title));
    containerLayout->addWidget(pane);

    containerWidget->setLayout(containerLayout);
    tab->addWidget(containerWidget);

    //triggered signal is not emitted when changing status programmatically
    connect(action, SIGNAL(triggered(bool)), this, SLOT(buttonToggled(bool)));
}

void InfoPanel::addInfoWidget(QWidget *w)
{
    toolbar->addWidget(w);
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

void InfoPanel::setCurrentPane(InfoPane *pane)
{
    int ix = indexOf(pane);
    Q_ASSERT(ix!=-1);

    setCurrentIndex(ix);
}

int InfoPanel::indexOf(InfoPane *pane) const
{
    return panes.indexOf(pane);
}

void InfoPanel::closePane(InfoPane *pane)
{
    int currentIx = getCurrentIndex();
    if(currentIx==-1){
        return;
    }

    if(currentIx == indexOf(pane)){
        closePanel();
    }
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

void InfoPanel::createToolbar()
{
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(12,12));
    WidgetHelper::changeFontSize(toolbar, -1);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setContentsMargins(0,0,0,0);

    placeholderAction=WidgetHelper::addStretchToToolbar(toolbar);
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
