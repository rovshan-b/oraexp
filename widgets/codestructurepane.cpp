#include "codestructurepane.h"
#include "codestructurewidget.h"
#include <QtGui>

CodeStructurePane::CodeStructurePane(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2,0,0,0);
    mainLayout->setSpacing(5);

    structureWidget = new CodeStructureWidget();
    mainLayout->addWidget(structureWidget);

    setLayout(mainLayout);
}

QSize CodeStructurePane::sizeHint() const
{
    return QSize(200, 200);
}
