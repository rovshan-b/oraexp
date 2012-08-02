#include "extentsizeeditorwidget.h"
#include <QtGui>

ExtentSizeEditorWidget::ExtentSizeEditorWidget(bool displayUnitCombo,
                                               bool addUnlimitedOption,
                                               QWidget *parent) :
    QWidget(parent), extentEditor(0), extentEditorCombo(0), extentUnitCombo(0)
{
    QHBoxLayout *layout=new QHBoxLayout();

    if(addUnlimitedOption){
        extentEditorCombo=new QComboBox();
        extentEditorCombo->setEditable(true);
        extentEditorCombo->addItem("");
        extentEditorCombo->addItem(tr("Unlimited"));
        extentEditorCombo->setCurrentIndex(0);
        layout->addWidget(extentEditorCombo, 1);
    }

    extentEditor=addUnlimitedOption? extentEditorCombo->lineEdit() : new QLineEdit();
    if(!addUnlimitedOption){
        QIntValidator *extentIsIntegerValidator=new QIntValidator(this);
        extentIsIntegerValidator->setBottom(0);
        extentEditor->setValidator(extentIsIntegerValidator);
        layout->addWidget(extentEditor, 1);
    }

    if(displayUnitCombo){
        extentUnitCombo=new QComboBox();
        extentUnitCombo->addItem(tr("Bytes"));
        extentUnitCombo->addItem(tr("MB"));
        extentUnitCombo->addItem(tr("GB"));
        layout->addWidget(extentUnitCombo);
    }

    /*if(displayUnlimitedCheckBox){
        unlimitedCheckBox=new QCheckBox(tr("Unlimited"));
        layout->addWidget(unlimitedCheckBox);

        connect(unlimitedCheckBox, SIGNAL(clicked(bool)), this, SLOT(unlimitedCheckBoxClicked(bool)));
    }*/

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);

    setLayout(layout);

    connect(extentEditor, SIGNAL(editingFinished()), this, SIGNAL(changed()));
    //if(displayUnlimitedCheckBox){
    //    connect(unlimitedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
    //}
    if(addUnlimitedOption){
        connect(extentEditorCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
    }
    if(displayUnitCombo){
        connect(extentUnitCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
    }
}

/*void ExtentSizeEditorWidget::unlimitedCheckBoxClicked(bool selected)
{
    extentEditor->setEnabled(!selected);

    if(extentUnitCombo!=0){
        extentUnitCombo->setEnabled(!selected);
    }

    //if(selected){
    //    extentEditor->setText("");
    //}
}*/

QString ExtentSizeEditorWidget::text() const
{
    return extentEditor->text();
}

void ExtentSizeEditorWidget::setText(const QString &text)
{
    if(extentEditorCombo){
        extentEditorCombo->setItemText(0, text);
        extentEditorCombo->setCurrentIndex(0);
    }else{
        extentEditor->setText(text);
    }
}

OraExp::ExtentUnit ExtentSizeEditorWidget::unit() const
{
    return (OraExp::ExtentUnit)extentUnitCombo->currentIndex();
}

void ExtentSizeEditorWidget::setUnit(OraExp::ExtentUnit unit)
{
    extentUnitCombo->setCurrentIndex(unit);
}

bool ExtentSizeEditorWidget::isUnlimited() const
{
    //return unlimitedCheckBox->isChecked();
    return extentEditor->text()==tr("Unlimited");
}

void ExtentSizeEditorWidget::setUnlimited(bool unlimited)
{
    //unlimitedCheckBox->setChecked(unlimited);
    //unlimitedCheckBoxClicked(unlimited);
    if(unlimited){
        extentEditorCombo->setCurrentIndex(1);
    }
}

QLineEdit *ExtentSizeEditorWidget::lineEdit() const
{
    return this->extentEditor;
}

//QCheckBox *ExtentSizeEditorWidget::checkBox() const
//{
//    return this->unlimitedCheckBox;
//}

QComboBox *ExtentSizeEditorWidget::editorComboBox() const
{
    return extentEditorCombo;
}
