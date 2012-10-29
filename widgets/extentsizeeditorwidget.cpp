#include "extentsizeeditorwidget.h"
#include "util/dbutil.h"
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
        extentEditorCombo->addItem(unlimitedText());
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
        extentUnitCombo->addItem(tr("KB"));
        extentUnitCombo->addItem(tr("MB"));
        extentUnitCombo->addItem(tr("GB"));
        layout->addWidget(extentUnitCombo);
    }

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);

    setLayout(layout);

    connect(extentEditor, SIGNAL(editingFinished()), this, SIGNAL(changed()));

    if(addUnlimitedOption){
        connect(extentEditorCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
    }
    if(displayUnitCombo){
        connect(extentUnitCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
    }
}

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

void ExtentSizeEditorWidget::parseText(const QString &text)
{
    Q_ASSERT(extentUnitCombo);

    bool unlimited;
    qulonglong quota;
    OraExp::ExtentUnit units;
    DbUtil::parseExtentSize(text.trimmed(), &unlimited, &quota, &units);

    if(unlimited){
        setUnlimited(unlimited);
        return;
    }

    setText(QString::number(quota));
    setUnit(units);
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
    return extentEditor->text()==unlimitedText();
}

void ExtentSizeEditorWidget::setUnlimited(bool unlimited)
{
    if(unlimited){
        extentEditorCombo->setCurrentIndex(1);
    }
}

bool ExtentSizeEditorWidget::isEmpty() const
{
    return extentEditor->text().trimmed().isEmpty();
}

QLineEdit *ExtentSizeEditorWidget::lineEdit() const
{
    return this->extentEditor;
}

QComboBox *ExtentSizeEditorWidget::editorComboBox() const
{
    return extentEditorCombo;
}

void ExtentSizeEditorWidget::clear()
{
    setText("");
    if(extentUnitCombo){
        extentUnitCombo->setCurrentIndex(0);
    }
}

void ExtentSizeEditorWidget::setDelegateMode()
{
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(0);
    if(extentEditorCombo){
        extentEditorCombo->setFrame(0);
        setFocusProxy(extentEditorCombo);
    }else{
        extentEditor->setFrame(0);
        setFocusProxy(extentEditor);
    }

    if(extentUnitCombo){
        extentUnitCombo->setFrame(0);
    }
}

QString ExtentSizeEditorWidget::getExtentSizeClause() const
{
    if(isEmpty()){
        return "";
    }

    return DbUtil::getExtentSizeClause(isUnlimited(), text().toInt(), unit());
}
