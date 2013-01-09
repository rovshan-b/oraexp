#include "bindparameditorwidget.h"
#include "util/iconutil.h"
#include <QtGui>

BindParamEditorWidget::BindParamEditorWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout=new QHBoxLayout();

    paramTypeCombo=new QComboBox();
    paramTypeCombo->addItem(tr("String/Number"));
    paramTypeCombo->addItem(tr("Date"));
    paramTypeCombo->addItem(tr("Cursor"));
    mainLayout->addWidget(paramTypeCombo);

    valueEditor=new QComboBox();
    valueEditor->setEditable(true);
    valueEditor->lineEdit()->setPlaceholderText(tr("null"));
    valueEditor->setMinimumWidth(200);
    mainLayout->addWidget(valueEditor);

    paramDirectionCombo=new QComboBox();
    paramDirectionCombo->addItem(IconUtil::getIcon("right_arrow"), tr("In"));
    paramDirectionCombo->addItem(IconUtil::getIcon("left_arrow"), tr("Out"));
    paramDirectionCombo->addItem(IconUtil::getIcon("left_right_arrows"), tr("InOut"));
    mainLayout->addWidget(paramDirectionCombo);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void BindParamEditorWidget::setFocusToEditor()
{
    valueEditor->setFocus();
}
