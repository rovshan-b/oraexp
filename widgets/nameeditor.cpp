#include "nameeditor.h"
#include "connectivity/dbconnection.h"

NameEditor::NameEditor(QWidget *parent) :
    QLineEdit(parent)
{

}

void NameEditor::setConnection(DbConnection *db)
{
    setMaxLength(db->getMaxIdentifierLength());
}
