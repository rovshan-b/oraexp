#include "dbobjectcreatorbottompane.h"
#include "codeeditor/codeeditor.h"
#include "util/widgethelper.h"
#include <QtGui>

DbObjectCreatorBottomPane::DbObjectCreatorBottomPane(QWidget *parent) :
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

void DbObjectCreatorBottomPane::setEditorText(const QString &text)
{
    WidgetHelper::appendToPlainTextEdit(codeEditor, text, true);
}

void DbObjectCreatorBottomPane::appendToEditor(const QString &text)
{
    WidgetHelper::appendToPlainTextEdit(codeEditor, text, false);
}

QStringList DbObjectCreatorBottomPane::getDdl() const
{
    return codeEditor->getSemicolonSeparated();
}

