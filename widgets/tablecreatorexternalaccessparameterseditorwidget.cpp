#include "tablecreatorexternalaccessparameterseditorwidget.h"
#include "codeeditor/codeeditor.h"
#include <QtGui>

TableCreatorExternalAccessParametersEditorWidget::TableCreatorExternalAccessParametersEditorWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    editor=new CodeEditor();
    layout->addWidget(editor);

    setLayout(layout);

    connect(editor, SIGNAL(textChanged()), this, SIGNAL(ddlChanged()));
}

void TableCreatorExternalAccessParametersEditorWidget::setAccessParams(const QString &accessParams)
{
    editor->setPlainText(accessParams);
}

QString TableCreatorExternalAccessParametersEditorWidget::getAccessParams() const
{
    return editor->toPlainText();
}
