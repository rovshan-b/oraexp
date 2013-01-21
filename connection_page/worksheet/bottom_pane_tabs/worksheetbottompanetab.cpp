#include "worksheetbottompanetab.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include <QtGui>

WorksheetBottomPaneTab::WorksheetBottomPaneTab(QWidget *parent) :
    QWidget(parent), pinned(false)
{
}

bool WorksheetBottomPaneTab::isPinned() const
{
    return pinned;
}

void WorksheetBottomPaneTab::setPinned(bool pinned)
{
    this->pinned=pinned;
}

void WorksheetBottomPaneTab::createToolbar()
{
    toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addAction(IconUtil::getIcon("pin"), tr("Pin"), this, SLOT(setPinned(bool)))->setCheckable(true);

    toolbar->addSeparator();

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}
