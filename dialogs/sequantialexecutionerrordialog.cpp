#include "sequantialexecutionerrordialog.h"
#include "util/widgethelper.h"
#include <QtGui>

SequantialExecutionErrorDialog::SequantialExecutionErrorDialog(const QString &title, const OciException &exception, const QString &code, QWidget *parent) :
    DescriptiveErrorDialog(title, exception, code, parent)
{
    QString errorMessage = exception.getErrorMessage();

    int colonIx = errorMessage.indexOf(':');
    QString errorPrefix = colonIx!=-1 ? errorMessage.left(colonIx) : QString::number(exception.getErrorCode());

    QVBoxLayout *radioLayout = new QVBoxLayout();

    ignoreAllRadio = new QRadioButton(tr("Ignore all errors"));
    ignoreCurrentRadio = new QRadioButton(tr("Ignore this error"));
    ignoreCurrentRadio->setChecked(true);
    ignoreCurrentCodeRadio = new QRadioButton(tr("Ignore all occurences of error %1").arg(errorPrefix));
    abortRadio = new QRadioButton(tr("Abort"));

    radioLayout->addWidget(ignoreAllRadio);
    radioLayout->addWidget(ignoreCurrentRadio);
    radioLayout->addWidget(ignoreCurrentCodeRadio);
    radioLayout->addWidget(abortRadio);

    radioGroup = new QButtonGroup(this);
    radioGroup->addButton(ignoreAllRadio, 0);
    radioGroup->addButton(ignoreCurrentRadio, 1);
    radioGroup->addButton(ignoreCurrentCodeRadio, 2);
    radioGroup->addButton(abortRadio, 3);

    QGroupBox *groupBox = WidgetHelper::createGroupBox(radioLayout, tr("Select action"));

    this->addWidget(groupBox);
}

void SequantialExecutionErrorDialog::setErrorAction(SequantialExecutionErrorDialog::ErrorAction action)
{
    radioGroup->button((int)action)->setChecked(true);
}

SequantialExecutionErrorDialog::ErrorAction SequantialExecutionErrorDialog::getErrorAction() const
{
    return (ErrorAction)radioGroup->checkedId();
}
