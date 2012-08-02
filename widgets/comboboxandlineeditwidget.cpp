#include "comboboxandlineeditwidget.h"
#include <QtGui>

ComboBoxAndLineEditWidget::ComboBoxAndLineEditWidget(bool onlyNumericInput, const QString &placeholderText, const QStringList &stringsToDisable, QWidget *parent) :
    QWidget(parent), placeholderText(placeholderText), stringsToDisable(stringsToDisable)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    dropdown=new QComboBox();
    layout->addWidget(dropdown);

    editor=new QLineEdit();
    layout->addWidget(editor);

    if(onlyNumericInput){
        QIntValidator *validator=new QIntValidator(this);
        editor->setValidator(validator);
    }

    comboBoxIndexChanged();
    connect(dropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIndexChanged()));

    setLayout(layout);

    connect(editor, SIGNAL(editingFinished()), this, SIGNAL(changed()));
}

QComboBox *ComboBoxAndLineEditWidget::comboBox() const
{
    return this->dropdown;
}

QLineEdit *ComboBoxAndLineEditWidget::lineEdit() const
{
    return this->editor;
}

void ComboBoxAndLineEditWidget::comboBoxIndexChanged()
{
    QString currentText=dropdown->currentText();
    bool disable = currentText.isEmpty() || currentText==tr("Disable") || stringsToDisable.contains(currentText);
    editor->setEnabled(!disable);

    if(!disable && !placeholderText.isEmpty() && this->isEnabled()){
        editor->setPlaceholderText(placeholderText);
    }else{
        editor->setPlaceholderText("");
    }

    emit changed();
}

void ComboBoxAndLineEditWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);

    if(event->type()==QEvent::EnabledChange){
        comboBoxIndexChanged();
    }
}
