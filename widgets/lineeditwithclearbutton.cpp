#include "lineeditwithclearbutton.h"
#include "util/iconutil.h"
#include <QtGui>

LineEditWithClearButton::LineEditWithClearButton(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();

    editor = new QLineEdit();
    editor->setPlaceholderText(tr("filter"));
    mainLayout->addWidget(editor);

    QToolButton *clearButton = new QToolButton();
    clearButton->setText(tr("Clear filter"));
    clearButton->setToolTip(tr("Clear filter"));
    clearButton->setIcon(IconUtil::getIcon("clear"));
    clearButton->setAutoRaise(true);
    mainLayout->addWidget(clearButton);

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);
    setLayout(mainLayout);

    connect(clearButton, SIGNAL(clicked()), editor, SLOT(clear()));
}
