#include "plaintexteditordialog.h"
#include "ui_plaintexteditordialog.h"

PlainTextEditorDialog::PlainTextEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlainTextEditorDialog)
{
    ui->setupUi(this);

    ui->plainTextEdit->setTabChangesFocus(true);
}

PlainTextEditorDialog::~PlainTextEditorDialog()
{
    delete ui;
}

QString PlainTextEditorDialog::getEditorText() const
{
    return ui->plainTextEdit->toPlainText();
}

void PlainTextEditorDialog::setEditorText(const QString &text)
{
    ui->plainTextEdit->setPlainText(text);
}
