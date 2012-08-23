#include "nameeditor.h"
#include "connectivity/dbconnection.h"
#include "defines.h"

NameEditor::NameEditor(QWidget *parent) :
    QLineEdit(parent)
{
    setMaxLength(MAX_IDENTIFIER_LENGTH);
}
/*
void NameEditor::setConnection(DbConnection *db)
{
    setMaxLength(db->getMaxIdentifierLength());
}*/
