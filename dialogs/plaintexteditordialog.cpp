#include "plaintexteditordialog.h"
#include "util/dialoghelper.h"
#include "codeeditor/codeeditor.h"
#include <QtGui>

PlainTextEditorDialog::PlainTextEditorDialog(QWidget *parent, bool useCodeEditor) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    editor = useCodeEditor ?  new CodeEditor() : new QPlainTextEdit();
    mainLayout->addWidget(editor);

    mainLayout->addWidget(DialogHelper::createButtonBox(this));

    setLayout(mainLayout);

    resize(400, 300);
}

void PlainTextEditorDialog::setEditorText(const QString &text)
{
    editor->setPlainText(text);
}

QString PlainTextEditorDialog::getEditorText() const
{
    return editor->toPlainText();
}
