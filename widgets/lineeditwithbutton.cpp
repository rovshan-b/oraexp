#include "lineeditwithbutton.h"
#include <QtGui>
#include <QtDebug>

LineEditWithButton::LineEditWithButton(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout=new QHBoxLayout();
    editor=new LineEditWithAutocompleter();
    button=new QPushButton(tr("..."));
    button->setMaximumWidth(button->fontMetrics().width("A")*3);

    layout->addWidget(editor);
    layout->addWidget(button);

    layout->setStretch(0, 1);
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    connect(button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

    setFocusProxy(editor);
    setBackgroundRole(palette().Base);
}

LineEditWithAutocompleter *LineEditWithButton::lineEdit() const
{
    return editor;
}

QPushButton *LineEditWithButton::browseButton() const
{
    return button;
}

void LineEditWithButton::browseButtonClicked()
{
    emit buttonClicked(this);
}
