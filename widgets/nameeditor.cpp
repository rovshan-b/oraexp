#include "nameeditor.h"
#include "defines.h"

NameEditor::NameEditor(QWidget *parent) :
    QLineEdit(parent)
{
    setMaxLength(MAX_IDENTIFIER_LENGTH);
}
