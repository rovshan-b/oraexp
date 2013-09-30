#include "codeviewerdialog.h"
#include "codeeditor/codeeditor.h"
#include <QtGui>

CodeViewerDialog::CodeViewerDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    editor = new CodeEditor(false);
    mainLayout->addWidget(editor);

    setLayout(mainLayout);

    resize(650, 400);
}

void CodeViewerDialog::setCode(const QString &code)
{
    editor->setPlainText(code);
    editor->setReadOnly(true);
    editor->document()->setModified(false);
}
