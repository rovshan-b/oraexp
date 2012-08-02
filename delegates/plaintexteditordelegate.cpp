#include "plaintexteditordelegate.h"
#include "dialogs/plaintexteditordialog.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include <QtGui>

PlainTextEditorDelegate::PlainTextEditorDelegate(const QString &dialogTitle, QObject *parent) :
    LineEditWithButtonDelegate(false, parent),
    dialogTitle(dialogTitle)
{
}

void PlainTextEditorDelegate::buttonClicked(LineEditWithButton *senderWidget)
{
    PlainTextEditorDialog dialog(senderWidget->window());
    dialog.setWindowTitle(this->dialogTitle);
    dialog.setEditorText(senderWidget->lineEdit()->text());

    if(dialog.exec()){
        senderWidget->lineEdit()->setText(dialog.getEditorText());
        emit commitData(senderWidget);
    }
}
