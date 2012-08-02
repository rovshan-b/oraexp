#include "tablecreatorbottompane.h"
#include "codeeditor/codeeditor.h"
#include "util/widgethelper.h"
#include <QtGui>

TableCreatorBottomPane::TableCreatorBottomPane(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    ddlGroupBox=new QGroupBox(tr("DDL"));
    QVBoxLayout *groupBoxLayout=new QVBoxLayout();
    codeEditor=new CodeEditor();
    codeEditor->setReadOnly(true);
    groupBoxLayout->addWidget(codeEditor);
    ddlGroupBox->setLayout(groupBoxLayout);
    layout->addWidget(ddlGroupBox);

    setLayout(layout);
}

void TableCreatorBottomPane::setEditorText(const QString &text)
{
    WidgetHelper::appendToPlainTextEdit(codeEditor, text, true);
}

void TableCreatorBottomPane::appendToEditor(const QString &text)
{
    WidgetHelper::appendToPlainTextEdit(codeEditor, text, false);
}

QStringList TableCreatorBottomPane::getDdl() const
{
    return codeEditor->getSemicolonSeparated();
}
