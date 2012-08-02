#include "programuniteditor.h"
#include "programcodeeditor.h"

#include <QtGui>

ProgramUnitEditor::ProgramUnitEditor(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    editor=new ProgramCodeEditor();

    layout->addWidget(editor);
    setLayout(layout);
}

void ProgramUnitEditor::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    setBusy(false);
}
