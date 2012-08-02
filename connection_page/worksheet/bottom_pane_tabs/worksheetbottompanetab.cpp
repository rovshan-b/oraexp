#include "worksheetbottompanetab.h"

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
